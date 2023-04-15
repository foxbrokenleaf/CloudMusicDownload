#include"CloudMusicDown.h"
int main() {
	string url;
	quees list;
	cout << "Please Enter Url:";
	cin >> url;
	url.erase(url.find('&'));
	Gfind(url,list);
	list.show();
	system("PAUSE");
	remove("temp.txt");
	remove("temp");
	return 0;
}