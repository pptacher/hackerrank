#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <stack>
#include <set>
#include <queue>
#include <unordered_map>
#include <bitset>
#include <array>
#include <numeric>

using std::string;
typedef unsigned int uint;
void dc3(std::vector<int>&, std::vector<int>&);
void lcp(string& s, std::vector<int>&,std::vector<int>&);
int max_function(std::vector<int>&);

int main(){

  //string s = "monsoonnomnoms";
  string s ="aacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccbaacbbabbabbbbbaaaaaaabbbbcacacbcabaccaabbbcaaabbccccbbbcbccccbbcaabaaabcbaacbcbaccaaaccbccbcaacbaccb";
  //string s="ab";

//string s = "abcabcddd";
//string s = "abcdefghi";
//string s = "aaaaaa";
  std::vector<int> u;
  for(uint i=0; i<s.length(); ++i){
    u.push_back(s[i]-'a');
  }
  std::vector<int> v(u.size(),0);

  dc3(u,v);
  std::vector<int> h(u.size()-1,0);
  lcp(s,v,h);

  int res = max_function(h);

  std::cout << res << std::endl;


}

int max_function(std::vector<int>& h){
  int n = h.size();
  std::stack<std::pair<int,int>> st;
  int res(0);
  h.push_back(0);

  for(int i=0; i<n+1; ++i){
    int indx(i);
      while(!st.empty() && h[i]<st.top().first){
          res = std::max(res,(i-st.top().second+1)*st.top().first);
          indx = st.top().second;
          st.pop();
      }
      st.push({h[i],indx});
  }

  return std::max(res,n);

}

void lcp(string& s, std::vector<int>& v, std::vector<int>& h){
  int n = v.size();
  std::vector<int> pos(n,0);
  for(int i=0; i<n; ++i){
    pos[v[i]] = i;
  }

  int k(0);
  for(int i=0; i<n-1; ++i){
    int j = pos[v[i]-1];
    while(i+k<n-1 && s[i+k]==s[j+k]){
      ++k;
    }
    h[v[i]-1] = k;
    if(k>0){
      --k;
    }
  }
}

void dc3(std::vector<int>& u, std::vector<int>& v){

  int n = u.size();

  if(n<=2){
		std::iota(v.begin(),v.end(),1);
		if(n==2 && u[1]<=u[0]){
			std::swap(v[0],v[1]);
		}
    return;
  }

	u.push_back(-1);
  v.push_back(0);
	++n;

  int n1((n-1+2)/3);
  int n2((n-2+2)/3);

  // build u[1...]+u[2...].
  std::vector<std::array<int,4>> w(n1+n2,std::array<int,4>());
  u.push_back(-1);
  u.push_back(-1);
  int mxm(u[0]);
  for(int i=0; i<n1; i++){
    w[i][0] = u[3*i+1];
    w[i][1] = u[3*i+2];
    w[i][2] = u[3*i+3];
    w[i][3] = i;
    mxm = std::max(mxm,std::max(u[3*i+1],std::max(u[3*i+2],u[3*i+3])));

  }
  for(int i=0; i<n2; i++){
    w[i+n1][0] = u[3*i+2];
    w[i+n1][1] = u[3*i+3];
    w[i+n1][2] = u[3*i+4];
    w[i+n1][3] = i+n1;
  }

  // radix sort blocks of 3.
  u.pop_back();
  u.pop_back();
  std::vector<std::array<int,4>> w1(n1+n2,std::array<int,4>());
  for(int k=0; k<3; ++k){
      std::vector<int> c((uint)mxm+2,0);

      for(int i=0; i<n1+n2; ++i){
        c[w[i][2-k]+1]++;
      }
      for(uint i=1; i<(uint)mxm+2; ++i){
        c[i] += c[i-1];
      }
      for(int i=n1+n2-1; i>=0; --i){
        w1[--c[w[i][2-k]+1]] = w[i];
      }
      std::swap(w1,w);

  }

  std::vector<int> u1(n1+n2,0);
  int count(0);

  for(int i=0; i<n1+n2; ++i){
    if(i>0 && (w[i][0]!=w[i-1][0] || w[i][1]!=w[i-1][1] || w[i][2]!=w[i-1][2] )){
      ++count;
    }
    u1[w[i][3]] = count;
  }

  // recursively compute suffix array for array of size 2n/3.
  std::vector<int> v2(n1+n2,0);
  dc3(u1,v2);

  // radix sort element at indices multiples of 3.
	std::vector<std::array<int,3>> u2(n-n1-n2,std::array<int,3>());
	for(int i=0; i<n-n1-n2; ++i){
		u2[i] = {{u[3*i],3*i+1<n?v2[i]:-1,3*i}};
	}
	std::vector<std::array<int,3>> u3(n-n1-n2,std::array<int,3>());
  mxm  = std::max(mxm,n1+n2);

	for(int k=0; k<2; ++k){
		std::vector<int> c((uint)mxm+2,0);

		for(int i=0; i<n-n1-n2; ++i){
			c[u2[i][1-k]+1]++;
		}
		for(uint i=1; i<(uint)mxm+2; ++i){
			c[i] += c[i-1];
		}
		for(int i=n-n1-n2-1; i>=0; --i){
			u3[--c[u2[i][1-k]+1]] = u2[i];
		}
		std::swap(u2,u3);

	}

  // merge.
	int i1(0), i12(0);
  std::vector<int> v1(n1+n2,0);
  for(int i=0; i<n1; ++i){
    v1[v2[i]-1] = 3*(i) + 1;
  }
  for(int i=0; i<n2; ++i){
    v1[v2[n1+i]-1] = 3*(i) + 2;
  }

	for(int i=0; i<n; ++i){

    if(i12==n1+n2){
        v[u2[i1++][2]] = i;
    }
    else if(i1==n-n1-n2){
       v[v1[i12++]] = i;
    }
    else if(u[u2[i1][2]]<u[v1[i12]] ||
        (u[u2[i1][2]] == u[v1[i12]] && v1[i12]%3==1 && (u2[i1][2]+1==n || (v1[i12]+1<n && v2[u2[i1][2]/3] < v2[n1+v1[i12]/3])) ) ||
         (u[u2[i1][2]] == u[v1[i12]] && v1[i12]%3==2 && (u[u2[i1][2]+1] < u[v1[i12]+1]||  (u[u2[i1][2]+1] == u[v1[i12]+1] && (u2[i1][2]+2 == n || (v1[i12]+2<n && v2[u2[i1][2]/3+n1]<v2[v1[i12]/3+1])))) ) ){

        v[u2[i1++][2]] = i;
    }
    else{
        v[v1[i12++]] = i;
    }
	}

}
