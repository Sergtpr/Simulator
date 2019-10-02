#pragma once


#include <string>
#include <set>

class FileMan {
public:
	std::string get_main_dir() const;
	std::string get_geom_fn() const;
	std::string get_conf_fn() const;
	std::set<std::string> get_bfield_objs() const;
	FileMan(const std::string& filename, const std::string& main);
private:
	std::string main_dir, geom_fn, conf_fn;
	std::set<std::string> bfield_objs;

};