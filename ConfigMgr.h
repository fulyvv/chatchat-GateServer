#pragma once
#include "const.h"

struct SectionInfo {
	SectionInfo() {};
	~SectionInfo() { _section_data.clear(); };

	//�������캯��
	SectionInfo(const SectionInfo& other) {
		_section_data = other._section_data;
	}

	//��ֵ���캯��
	SectionInfo& operator=(const SectionInfo& other) {
		if (this != &other) {
			_section_data = other._section_data;
		}
		return *this;
	}

	std::map<std::string, std::string> _section_data;

	//����[]Զ���
	std::string operator[](const std::string& key) {
		if (_section_data.find(key) == _section_data.end()) {
			return "";
		}
		return _section_data[key];
	}

};
class ConfigMgr
{
public:
	~ConfigMgr() {
		_config_map.clear();
	
	}

	SectionInfo operator[](const std::string& section) {
		if (_config_map.find(section) == _config_map.end()) {
			return SectionInfo();
		}
		return _config_map[section];
	}

	ConfigMgr();

	//��������
	ConfigMgr(const ConfigMgr& other);

	//������ֵ
	ConfigMgr& operator=(const ConfigMgr& other);

private:
	std::map<std::string, SectionInfo> _config_map;//section����,section��Ϣ

};

