#include"CloudMusicDown.h"
//定义头文件函数
int num = 1;
quees::quees() {
	head = new song;
	head->Name = "null";
	head->Id = "null";
	head->next = nullptr;
	num = 0;
}
quees::~quees() {

}
//添加节点定义
void quees::add(string n, string id) {
	song *temp = new song;
	temp->Name = n;
	temp->Id = id;
	temp->next = nullptr;
	song *thead = head;
	if (num == 0) head = temp;
	for (int i = 1; i <= num; i++) {
		if (thead->next == nullptr) break;
		thead = thead->next;
	}
	thead->next = temp;
	num++;
}
//显示函数定义（用于DEBUG）
void quees::show() {
	song *temp = head;
	for (int i = 1; i <= num; i++) {
		cerr << "Name:" << temp->Name << endl
			 << "ID:" << temp->Id << endl;
		if (temp->next == nullptr) break;
		temp = temp->next;
	}
}
//获取song中的歌名和ID定义
string quees::get(int n) {
	song *temp = head;
	for (int i = 1; i <= n && i <= num; i++){
		if (temp->next == nullptr) break;
		temp = temp->next;
	}
	return temp->Name;
}
//初始化curl定义
CURL *curlinit() {
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl = curl_easy_init();
	return curl;
}
//释放curl定义
void curlcleanup(CURL *curl) {
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}
//curl操作函数定义（初始化，GET请求，释放）
void curl_fun(string url,string loadpath,int num) {
	CURL *curl = curlinit();
	CURLcode res;
	char* lasturl = new char[10];
	string t1 = "http://music.163.com/song/media/outer/url?id=" + url + ".mp3";
	string t2 = loadpath +".mp3";
	char *ptr = new char[t2.length() + 1 ];
	strcpy(ptr, t2.c_str());
	FILE *fp = fopen(ptr, "wb");
	if (fp == NULL) cerr << "File No ok!" << endl;
	else {
		curl_easy_setopt(curl, CURLOPT_URL, t1.c_str());
		res = curl_easy_perform(curl);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &lasturl);
		curlwrite(curl, fp);
		curlcleanup(curl);
		fclose(fp);
		delete[] ptr;
	}
}
//不可在未初始化curl的情况下调用该函数，写入函数定义
void curlwrite(CURL*curl, FILE *fp) {

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, r_write_call);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_perform(curl);
}
void Gfind(string url,quees &list) {
	CURL *curl = curlinit();
	CURLcode res = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	FILE *html = fopen("temp.txt", "wb");
	curlwrite(curl, html);
	curlcleanup(curl);
	fclose(html);
	wstring tname = utf8("temp.txt");
	wstring mode = utf8("r,ccs=UTF-8");
	FILE *fp = _wfopen(tname.c_str(), mode.c_str());
	ofstream out("temp", ios::ios_base::out | ios::ios_base::binary);
	if (fp) {
		while (!feof(fp)) {
			wchar_t *wchar = new wchar_t[2];
			fgetws(wchar, 2, fp);
			wstring pstr = wchar;
			string ppstr = ansi(pstr);
			out.write(ppstr.c_str(), ppstr.length());
			delete[] wchar;
		}
	}
	fclose(fp);
	out.close();
	string temp,sign,name,id;
	ifstream file("temp", ios::ios_base::binary | ios::ios_base::in);
	ifstream signstr("sign", ios::ios_base::binary | ios::ios_base::in);
	getline(signstr, sign);
	signstr.close();
	while (!file.eof()) {
		getline(file, temp);
		if (temp.find(sign.c_str()) != -1) {
			file.close();
			string ppstr;
			for (int i = 33; i < temp.length();i++) ppstr += temp[i];
			temp = ppstr;
			ppstr.erase(0);
			for (int cheack = 1; cheack <= 10; cheack++) {
				for (int i = temp.find("song?id=") + 8; i < temp.find("\">"); i++) ppstr += temp[i];
				id = ppstr;
				ppstr.erase(0);
				for (int i = temp.find("\">") + 2; i < temp.length(); i++) ppstr += temp[i];
				temp = ppstr;
				ppstr.erase(0);
				for (int i = 0; i < temp.find("<"); i++) ppstr += temp[i];
				name = ppstr;
				ppstr.erase(0);
				list.add(name, id);
				curl_fun(id, name,num);
				num++;
			}
		}
	}
}
wstring utf8(const char *ptr) {
	int len = MultiByteToWideChar(CP_UTF8, 0, ptr, strlen(ptr), NULL, 0);
	wchar_t *str = new wchar_t[++len];
	MultiByteToWideChar(CP_UTF8, 0, ptr, strlen(ptr) + 1, str, len);
	wstring rstr = str;
	delete str;
	return rstr;
}
string ansi(wstring ptr) {
	int len = WideCharToMultiByte(CP_ACP, 0, ptr.c_str(), ptr.length(), NULL, 0, NULL, NULL);
	char *str = new char[++len];
	WideCharToMultiByte(CP_ACP, 0, ptr.c_str(), ptr.length() + 1, str, len, NULL, NULL);
	string rstr = str;
	delete str;
	return rstr;
}
//537854739 鹿乃-DAYBREAK FRONTLINE
//http://music.163.com/song/media/outer/url?id=537854739.mp3
//https://music.163.com/api/playlist/detail?id=歌单ID