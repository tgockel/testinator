#pragma once

#ifdef TESTPP_MAIN

int main(int argc, char* argv[])
{
  std::string outputterName;
  std::string testName;
  std::string suiteName;
  testpp::RunParams p;
  auto oflags = testpp::OF_COLOR|testpp::OF_QUIET_SUCCESS;

  for (int i = 1; i < argc; ++i)
  {
    std::string s = argv[i];

    {
      std::string option = "--output=";
      if (s.compare(0, option.size(), option) == 0)
      {
        outputterName = s.substr(option.size());
        continue;
      }
    }

    {
      std::string option = "--testName=";
      if (s.compare(0, option.size(), option) == 0)
      {
        testName = s.substr(option.size());
        continue;
      }
    }

    {
      std::string option = "--suiteName=";
      if (s.compare(0, option.size(), option) == 0)
      {
        suiteName = s.substr(option.size());
        continue;
      }
    }

    {
      std::string option = "--numChecks=";
      if (s.compare(0, option.size(), option) == 0)
      {
        char* end;
        p.m_numPropertyChecks = strtoul(s.substr(option.size()).c_str(), &end, 10);
        continue;
      }
    }

    {
      std::string option = "--seed=";
      if (s.compare(0, option.size(), option) == 0)
      {
        char* end;
        p.m_randomSeed = strtoul(s.substr(option.size()).c_str(), &end, 10);
        continue;
      }
    }

    {
      std::string option = "--alpha";
      if (s.compare(0, option.size(), option) == 0)
      {
        p.m_flags |= testpp::RF_ALPHA_ORDER;
        continue;
      }
    }

    {
      std::string option = "--verbose";
      if (s.compare(0, option.size(), option) == 0)
      {
        oflags &= ~(static_cast<std::underlying_type_t<testpp::OutputFlags>>(
                        testpp::OF_QUIET_SUCCESS));
        continue;
      }
    }

    {
      std::string option = "--nocolor";
      if (s.compare(0, option.size(), option) == 0)
      {
        oflags &= ~(static_cast<std::underlying_type_t<testpp::OutputFlags>>(
                        testpp::OF_COLOR));
        continue;
      }
    }

    {
      std::string option = "--help";
      if (s.compare(0, option.size(), option) == 0)
      {
        std::cout << "Usage: testpp [OPTION]..." << std::endl
                  << "Run all tests in randomized order by default." << std::endl
                  << std::endl
                  << "--testName=NAME    run only the named test" << std::endl
                  << "--suiteName=NAME   run only the tests in the named suite" << std::endl
                  << "--alpha            run tests in alphabetical order" << std::endl
                  << "--output=FORMAT    use the specified output formatter, e.g. TAP" << std::endl
                  << "--verbose          give verbose output (according to formatter)" << std::endl
                  << "--nocolor          output without ANSI color codes (according to formatter)"
                  << std::endl
                  << "--numChecks=N      number of checks to use for property tests" << std::endl
                  << "--seed=SEED        use SEED for property test randomization" << std::endl;
        return 0;
      }
    }

  }

  std::unique_ptr<testpp::Outputter> op = testpp::MakeOutputter(
      outputterName, static_cast<testpp::OutputFlags>(oflags));

  testpp::Results rs;
  if (!testName.empty())
    rs = testpp::RunTest(testName, p, op.get());
  else if (!suiteName.empty())
    rs = testpp::RunSuite(suiteName, p, op.get());
  else
    rs = testpp::RunAllTests(p, op.get());

  auto numPassed = count_if(rs.begin(), rs.end(),
                            [] (const testpp::Result& r) { return r.m_success; });
  auto total = static_cast<decltype(numPassed)>(rs.size());
  return total - numPassed;
}

#endif