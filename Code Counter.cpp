#pragma warning(disable:4996)
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<string.h>
#include<time.h>
#include<algorithm>
#include<vector>
#include<fstream>
#include<sstream>
#include<utility>
using namespace std;
void getFileList(char* folder, const char* tempPath);
string getFileName(const string& path);
bool isTempFile(const string& path);
bool isCppFile(const string& path);
size_t codeCalc(const string& path);
pair<size_t, size_t> Code_Counter(char* folder, const char* tempPath);
int main(void)
{
	char folder[256] = "C:\\Users\\18134\\source\\repos";
	const char* tempPath = "C:\\Users\\18134\\source\\repos\\Code Counter v1.5\\fileList.txt";
	double start = clock();
	pair<size_t, size_t> res = Code_Counter(folder, tempPath);
	printf("SumLine:[%d]\nSumFile:[%d]\n", res.first, res.second);
	printf("Time Cost:[%.2f]s\n", (clock() - start) / CLOCKS_PER_SEC);
	system("pause");
	return 0;
}
void getFileList(char* folder, const char* tempPath) {
	int len = strlen(folder);
	if (folder[len - 1] == '\\')folder[len - 1] = '\0';
	static char cmd[256];
	sprintf(cmd, "cmd /c dir /s /b \"%s\\*.cpp\" \"%s\\*.c\" \"%s\\*.txt\" \"%s\\*.h\"  > \"%s\"", folder, folder, folder, folder, tempPath);
	cout << "[Geting File List]...\n";
	system(cmd);
	cout << "[File List Get]\n";
}
string getFileName(const string& path) {
	int pos = path.find_last_of('\\');
	if (pos == string::npos)return "ERROR";
	return path.substr(pos + 1);
}
bool isTempFile(const string& path) {
	string filename = getFileName(path);
	transform(filename.begin(), filename.end(), filename.begin(), tolower);
	return filename.find("temp") != string::npos || filename.find("complete") != string::npos;
}
bool isCppFile(const string& path) {
	const int limit = 2;
	static const vector<string>features =
	{ "#include<","int main(","using namespace std;","return 0;","#pragma","public:","private:","std::","int ","void ","bool " };
	vector<bool>map(features.size(), false);
	ifstream ifs(path);
	if (!ifs)return false;
	static char line[1024];
	const int limitLine = 1000;
	int Line = 0;
	while (ifs.getline(line, sizeof(line))) {
		string str(line);
		int size = features.size();
		for (int i = 0; i < size; i++)
			if (str.find(features[i]) != string::npos)
				map[i] = true;
		Line++;
		if (Line > limitLine)break;
	}
	int sum = 0;
	for (auto i : map)
		sum += i;
	return sum >= limit;
}
size_t codeCalc(const string& path) {
	ifstream ifs(path);
	if (!ifs)return 0;
	static char line[1024];
	size_t sumLine = 0;
	while (ifs.getline(line, sizeof(line))) {
		istringstream iss(line);
		string str;
		iss >> str;
		if (str == "" || str.find("//") == 0)continue;
		sumLine++;
	}
	return sumLine;
}
pair<size_t,size_t> Code_Counter(char* folder, const char* tempPath) {
	getFileList(folder, tempPath);
	ifstream filelist(tempPath);
	if (!filelist)return { 0,0 };
	static char filepath[1024];
	size_t sumLine = 0;
	size_t sumFile = 0;
	while (filelist.getline(filepath, sizeof(filepath))) {
		if (isTempFile(filepath)) {
			printf("[%s] is a [TempFile]\n", filepath);
			continue;
		}
		if (!isCppFile(filepath)) {
			printf("[%s] is NOT a [CppFile]\n", filepath);
			continue;
		}
		sumFile++;
		sumLine += codeCalc(filepath);
	}
	return { sumLine,sumFile };
}
