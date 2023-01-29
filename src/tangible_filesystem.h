#pragma once

//----------------------------------------------------------------------------------------
//	Copyright © 2004 - 2019 Tangible Software Solutions, Inc.
//	This class can be used by anyone provided that the copyright notice remains intact.
//
//	This class is used to replace some static .NET file and folder method calls
//	with std::filesystem method calls.
//----------------------------------------------------------------------------------------
#include <string>
#include <filesystem>

class FileSystem
{
public:
	static void createDirectory(const std::string &path)
	{
		std::filesystem::create_directory(pathFromString(path));
	}

	static bool fileExists(const std::string &path)
	{
		return std::filesystem::is_regular_file(pathFromString(path));
	}

	static bool directoryExists(const std::string &path)
	{
		return std::filesystem::is_directory(pathFromString(path));
	}

	static std::string combine(const std::string &path1, const std::string &path2)
	{
		return (pathFromString(path1) / pathFromString(path2)).generic_wstring();
	}

	static bool isPathRooted(const std::string &path)
	{
		return pathFromString(path).has_root_path();
	}

	static std::string getFullPath(const std::string &path)
	{
		return std::filesystem::absolute(pathFromString(path)).generic_wstring();
	}

	static std::string getFileName(const std::string &path)
	{
		return std::filesystem::path(pathFromString(path)).filename().generic_wstring();
	}

	static std::string getDirectoryName(const std::string &path)
	{
		return std::filesystem::path(pathFromString(path)).parent_path().generic_wstring();
	}

	static std::string getCurrentDirectory()
	{
		return std::filesystem::current_path().generic_wstring();
	}

	static void copyFile(const std::string &path1, const std::string &path2)
	{
		std::filesystem::copy_file(pathFromString(path1), pathFromString(path2));
	}

	static void renamePath(const std::string &path1, const std::string &path2)
	{
		std::filesystem::rename(pathFromString(path1), pathFromString(path2));
	}

	static char preferredSeparator()
	{
		return std::filesystem::path::preferred_separator;
	}

private:
	static std::filesystem::path pathFromString(const std::string &path)
	{
		return std::filesystem::path(&path[0]);
	}
};
