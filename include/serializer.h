#pragma once


class Serializer
{
public:
	Serializer(const Serializer&) = delete;
	Serializer& operator=(const Serializer&) = delete;
	static Serializer& GetInstance();

	static void SaveData();
	static void LoadData();

private:
	Serializer() = default;
	~Serializer() = default;
};
