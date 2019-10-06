#pragma once

#include <string>
#include <set>

class FileMan {
public:
	std::string get_main_dir() const;
	std::string get_geom_fn() const;
	std::string get_conf_fn() const;
	std::string get_path() const;
	void set_output_path(const std::string& output);
	std::string get_output_path() const;
	std::set<std::string> get_bfield_objs() const;
	FileMan(const std::string& filename, const std::string& main);
private:
	std::string main_dir, geom_fn, conf_fn, path, output_path;
	std::set<std::string> bfield_objs;
};