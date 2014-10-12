// OpenCppCoverage is an open source code coverage for C++.
// Copyright (C) 2014 OpenCppCoverage
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"

#include "CppCoverage/Options.hpp"
#include "CppCoverage/ProgramOptions.hpp"

#include "CppCoverageTest/TestTools.hpp"

#include "Tools/Tool.hpp"

namespace cov = CppCoverage;

namespace CppCoverageTest
{
	namespace
	{
		const std::string optionShortPrefix = "-";		
	}
		
	//-------------------------------------------------------------------------
	TEST(OptionsParserTest, Default)
	{
		cov::OptionsParser parser;

		auto options = TestTools::Parse(parser, {});
		ASSERT_TRUE(options);
		ASSERT_FALSE(options->IsVerboseModeSelected());			
	}

	//-------------------------------------------------------------------------
	TEST(OptionsParserTest, Help)
	{
		cov::OptionsParser parser;
		std::wostringstream ostr;

		ASSERT_FALSE(TestTools::Parse(parser,
		{ optionShortPrefix + cov::ProgramOptions::HelpShortOption }, false, &ostr));
		
		ASSERT_NE(L"", ostr.str());
		ASSERT_FALSE(TestTools::Parse(parser,
		{ TestTools::OptionPrefix + cov::ProgramOptions::HelpOption }, false));
	}

	//-------------------------------------------------------------------------
	TEST(OptionsParserTest, Verbose)
	{
		cov::OptionsParser parser;

		ASSERT_TRUE(TestTools::Parse(parser,
		{ optionShortPrefix + cov::ProgramOptions::VerboseShortOption })->IsVerboseModeSelected());
		ASSERT_TRUE(TestTools::Parse(parser,
		{ TestTools::OptionPrefix + cov::ProgramOptions::VerboseOption })->IsVerboseModeSelected());
	}
		
	//-------------------------------------------------------------------------
	TEST(OptionsParserTest, WorkingDirectory)
	{
		cov::OptionsParser parser;
		const std::string folder = ".";

		auto options = TestTools::Parse(parser,
		{ TestTools::OptionPrefix + cov::ProgramOptions::WorkingDirectoryOption, folder });
		ASSERT_TRUE(options);

		const auto* workingDirectory = options->GetStartInfo().GetWorkingDirectory();

		ASSERT_NE(nullptr, workingDirectory);
		ASSERT_EQ(Tools::ToWString(folder), *workingDirectory);
	}

	//-------------------------------------------------------------------------
	TEST(OptionsParserTest, OutputDirectory)
	{
		cov::OptionsParser parser;
		const std::string folder = "Output";

		auto options = TestTools::Parse(parser,
		{ TestTools::OptionPrefix + cov::ProgramOptions::OutputDirectoryOption, folder });
		ASSERT_TRUE(options);

		auto outputDirectoryOption = options->GetOutputDirectoryOption();
		
		ASSERT_TRUE(outputDirectoryOption);
		ASSERT_EQ(folder, outputDirectoryOption->string());		
	}
	
	//-------------------------------------------------------------------------
	TEST(OptionsParserTest, Program)
	{
		cov::OptionsParser parser;		
		const std::string arg1 = "arg1";
		const std::string arg2 = "arg2";
		const std::vector<std::wstring> expectedArgs =
		{ Tools::ToWString(TestTools::ProgramToRun), Tools::ToWString(arg1), Tools::ToWString(arg2) };

		auto options = TestTools::Parse(parser, { TestTools::ProgramToRun, arg1, arg2 }, false);
		ASSERT_TRUE(options);

		const auto& startInfo = options->GetStartInfo();		
		ASSERT_EQ(TestTools::ProgramToRun, startInfo.GetPath().string());
		ASSERT_EQ(expectedArgs, startInfo.GetArguments());
	}

	//-------------------------------------------------------------------------
	TEST(OptionsParserTest, UnknownOption)
	{
		cov::OptionsParser parser;
		std::wostringstream ostr;
		ASSERT_FALSE(TestTools::Parse(parser, { "--unknownOption" }, true, &ostr));
		ASSERT_NE(L"", ostr.str());
	}
				
	//-------------------------------------------------------------------------
	TEST(OptionsParserTest, OptionOstream)
	{ 
		cov::OptionsParser parser;
		auto options = TestTools::Parse(parser, { TestTools::OptionPrefix + cov::ProgramOptions::SelectedSourcesOption, "source1" });
		
		std::wostringstream ostr;

		ASSERT_TRUE(options);
		ostr << *options;
		ASSERT_LT(L"", ostr.str());
	}
}