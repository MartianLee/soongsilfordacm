#include "DirectedGraph.h"

using namespace std;

DirectedGraph g(200);
unordered_map<int, bool> im;
map<string, int> s2i;
map<int, string> i2s;
void regi(const string &s)
{
	if (s2i.find(s) == s2i.end())
	{
		int idx = (int)s2i.size();
		i2s[idx] = s;
		s2i[s] = idx;
				im[idx]=true;
	}
}

void solve(const string &fname, const string &hdir)
{
	regi(fname);
	auto tmp = (fname.substr(fname.size()-2, 2) == ".h"?hdir:"") + fname;
	ifstream in((fname.substr(fname.size()-2, 2) == ".h"?hdir:"") + fname);
	char buf[10000];
	while (in.getline(buf, sizeof buf))
	{
		if (string(buf, 8) == "#include")
		{
			auto it = buf + 8;
			while (*it == ' ' || *it == '\t')
				it++;
			if (*it != '\"')
				continue;
			string s = it + 1;
			while(s.back() == ' ' || s.back()=='\t'||s.back() == '\r' || s.back()=='\n')
				s.pop_back();
			s.pop_back();
			if (s == "bits/stdc++.h")
				continue;
			solve(s, hdir);
			g.add_edge(s2i[fname], s2i[s]);
		}
	}
	in.close();
}

int main(int argc, char ** argv){
	string fn;
	string hdir;
	if (argc < 3)	{
		cout << "main source file: ";
		cin >> fn;
		cout << "header directory: ";
		cin >> hdir;
		if(hdir.back() != '/')
			hdir.push_back('/');
	}
	else if (argc == 3)	{
		fn = argv[1];
		if (fn.substr(fn.size() - 4, 4) != ".cpp")
			fn += ".cpp";
		hdir = argv[2];
		if(hdir.back() != '/')
			hdir.push_back('/');
	}
	else	{
		cout << "Usage1 : merger.exe" << endl;
		cout << "Usage2 : merger.exe <MAIN_SOURCE_FILE> <HEADER_PATH>" << endl;
		return 0;
	}
	solve(fn, hdir);
	auto ord = g.topo_sort();
	ofstream out("src/output.cpp");
	for (auto i : ord)
	{
				if(!im[i])
						continue;
		ifstream in((i2s[i].substr(i2s[i].size()-2, 2) == ".h"?hdir:"") + i2s[i]);
		char buf[10000];
		while (in.getline(buf, sizeof buf))
		{
			if (string(buf, strlen("#pragma once")) == "#pragma once")
				continue;
			if (string(buf, strlen("#include")) == "#include")
			{
				auto it = buf + 8;
				while (*it == ' ' || *it == '\t')
					it++;
				if (*it == '\"')
				{
					string s = it + 1;
					while (s.back() == ' ' || s.back() == '\t'||s.back() == '\r' || s.back()=='\n')
						s.pop_back();
					s.pop_back();
					if (s != "bits/stdc++.h")
						continue;
				}
			}
			out << buf << endl;
		}
		in.close();
	}
	out.close();

	return 0;
}