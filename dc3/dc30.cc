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
#include <algorithm>

using std::string;
typedef unsigned int uint;

void dc3(std::vector<int>&, std::vector<int>&);
void lcp(string& s, std::vector<int>&,std::vector<int>&);
void rmqInit(std::vector<int>& v, std::vector<std::vector<int>>& u,std::vector<int>& w);
int rmqQuery(std::vector<std::vector<int>>& u, std::vector<int>& w, int i, int j);
int match(string& s1, int j, string& s2, std::vector<int>& h, std::vector<int>& pos1r, std::vector<std::vector<int>>& u,std::vector<int>&, int&,int&,int& );
void preprocess(string&, std::vector<int>&, std::vector<int>&, std::vector<int>&, std::vector<std::vector<int>>&, std::vector<int>&, std::vector<int>&,bool);
void buildPalindrome(string&,string&,string&,std::vector<int>&,std::vector<int>&,std::vector<int>&,std::vector<std::vector<int>>&,std::vector<int>&,std::vector<int>&,std::vector<int>&,std::vector<int>&,std::vector<std::vector<int>>&,std::vector<int>&,std::vector<int>&,int&,int&,int&);

int  main() {

  //string s1("dczatfarqdkelalxzxillkfdvpfpxabqlngdscrentzamztvvcvrtcm"), s2("bqlizijdwtuyfrxolsysxlfebpolcmqsppmrfkyunydtmwbexsngxhwvroandfqjamzkpttslildlrkjoyrpxugiceahgiake");
  string s1("qquhuwqhdswxxrxuzzfhkplwunfagppcoildagktgdarveusjuqfistulgbglwmfgzrnyxryetwzhlnfewczmnoozlqatugmd");
  string s2("jwgzcfabbkoxyjxkatjmpprswkdkobdagwdwxsufeesrvncbszcepigpbzuzoootorzfskcwbqorvw");

  string s1r(s1), s2r(s2);
  std::reverse(s1r.begin(), s1r.end());
  std::reverse(s2r.begin(),s2r.end());

  std::vector<int> u1r, u2r, u1, u2;
  std::vector<int> v1r, v2r, v1, v2;
  std::vector<int> h1r, h2r, h1, h2;
  std::vector<std::vector<int>> rmq1r, rmq2r, rmq1, rmq2;
  std::vector<int> p1r, p2r, p1, p2;
  std::vector<int> w1r, w2r, w1, w2;
  preprocess(s2r,u2r,v2r,h2r,rmq2r,w2r,p2r,false);
  u2r.clear();p2r.clear();rmq2r.clear();h2r.clear();
  preprocess(s1,u1,v1,h1,rmq1,w1,p1,false);
  u1.clear();p1.clear();rmq1.clear();h1.clear();
  preprocess(s1r,u1r,v1r,h1r,rmq1r,w1r,p1r,true);
  u1r.clear();
  preprocess(s2,u2,v2,h2,rmq2,w2,p2,true);
  u2.clear();

  int j(-1), mxm(0),pal(0);
  buildPalindrome(s2,s1r,s2r,h1r,v1r,p1r,rmq1r,w1r,v2,h2,p2,rmq2,w2,v2r,j,pal,mxm);

  int j1(-1),mxm1(0), pal1(0);
  buildPalindrome(s1r,s2,s1,h2,v2,p2,rmq2,w2,v1r,h1r,p1r,rmq1r,w1r,v1,j1,pal1,mxm1);

  string res("");
  if(mxm1>mxm){
    res = s1.substr(s1.length()-j1-(mxm1-pal1)/2,(mxm1-pal1)/2+pal1) + (mxm1-pal1>0?s1r.substr(j1,(mxm1-pal1)/2):"");
  }
  else if (mxm>mxm1){
    res = (mxm-pal>0?s2r.substr(s2.length()-j-(mxm-pal)/2,(mxm-pal)/2):"") + s2.substr(j-pal,(mxm-pal)/2+pal);
  }
  else if (mxm==mxm1 && mxm>0){
    if(s1.compare(s1.length()-j1-(mxm1-pal1)/2,(mxm1-pal1)/2+(pal1+1)/2,s2r,s2.length()-j-(mxm-pal)/2,(mxm1-pal1)/2+(pal1+1)/2)<0){
      res = s1.substr(s1.length()-j1-(mxm1-pal1)/2,(mxm1-pal1)/2+pal1) + (mxm1-pal>0?s1r.substr(j1,(mxm1-pal1)/2):"");
    }
    else{
      res = (mxm-pal>0?s2r.substr(s2.length()-j-(mxm-pal)/2,(mxm-pal)/2):"") + s2.substr(j-pal,(mxm-pal)/2+pal);
    }
  }

  std::cout << res << std::endl;

  return 0;

}

void buildPalindrome(string& s2, string& s1r, string& s2r,
                        std::vector<int>& h1r,
                        std::vector<int>& v1r,
                        std::vector<int>& p1r,
                        std::vector<std::vector<int>>& rmq1r,
                        std::vector<int>& w1r,
                        std::vector<int>& v2,
                        std::vector<int>& h2,
                        std::vector<int>& p2,
                        std::vector<std::vector<int>>& rmq2,
                        std::vector<int>& w2,
                        std::vector<int>& v2r,
                        int& j,
                        int& pal,
                        int& mxm){

  j = -1;
  pal = 0;
  mxm = 0;
bool b(false);
  std::set<int> si1, si2;
  int hint(0), k1(0);
  int suff(0);
  std::vector<int> j1(s2.length(),0);
  for(uint i=0; i<s2.length(); ++i){
      if(k1>1){
          hint = v1r[p1r[suff]+1];
          suff = hint;
      }
      --k1;
      j1[i] = match(s2,i,s1r,h1r,p1r,rmq1r,w1r,suff,k1,hint);
      if(j1[i]>0){
          b = true;
      }
  }
  if(!b){
      return;
  }
  for(uint i=0; i<s2.length(); ++i){
      int j2(0);
    for(uint  ind: si1){
          if(ind<s2.length()-1 && s2r[ind+1]==s2r[s2.length()-i]){
            si2.insert(ind+1);
            j2 = ind+1-s2.length()+i+1;
          }
        }
        if(i>0){
          si2.insert(s2.length()-i);
          j2=  std::max(j2,1);
          if(i>1 && s2r[s2.length()-i]==s2r[s2.length()-i+1]){
            si2.insert(s2.length()-i+1);
            j2 = std::max(j2,2);
          }
        }
        std::swap(si1,si2);
        si2.clear();
      if(j1[i]>0 && static_cast<int>(i)+j1[i] >= mxm){
        //j2 = palindromes(s2r,s2,s2.length()-i,v2,h2,p2,rmq2,w2);
        int c(1);
        /*if(j2+2*j1==mxm){
            c = s2r.compare(s2.length()-i-(j2+2*j1-j2)/2,(j2+2*j1-j2)/2+(j2+1)/2,s2r,s2.length()-j-(mxm-pal)/2,(mxm-pal)/2+(pal+1)/2);
        }*/
        if((j2+2*j1[i]>mxm /*|| c < 0  */|| (j2+2*j1[i]==mxm && v2r[s2.length()-i-j1[i]]<v2r[s2.length()-j-(mxm-pal)/2]))){
          mxm = j2+2*j1[i];
          j = i;
          pal = j2;
        }
      }
    }

}

void preprocess(string& s,
                  std::vector<int>&u,
                  std::vector<int>& v,
                  std::vector<int>& h,
                  std::vector<std::vector<int>>& rmq,
                  std::vector<int>& w,
                  std::vector<int>& p,
                  bool b=true){

  for(uint i=0; i<s.length(); ++i){
    u.push_back(s[i]-'a');
  }
  v = std::vector<int>(u.size(),0);
  h = std::vector<int>(u.size(),0);

  dc3(u,v);
  if(!b){
      return;
  }
  lcp(s,v,h);
  rmqInit(h,rmq,w);

  p = std::vector<int>(u.size(),0);
  for(uint i=0; i<u.size(); ++i){
    p[v[i]] = i;
  }

}

int match(string& s1, int j, string& s2, std::vector<int>& h, std::vector<int>& pos, std::vector<std::vector<int>>& u,std::vector<int>& w, int& suffix, int& k1, int& hint){
  int n = s1.length();
  int m = s2.length()+1;
  int res(0);
  int lcp(k1>0?k1:0);
  int k(k1>0?k1:0);
  int a(0), b(m-1);
  int mid(k1>0?hint:a + (b-a)/2),mid1(mid);
  int last(mid);

  while(k+j<n && a<=b){

    lcp = mid==last?lcp:rmqQuery(u,w,std::min(mid,last),std::max(mid,last)-1);

    if(lcp>=k+1){
        if(mid1<mid){
          a = mid+1;
        }
        else{
          b = mid-1;
        }
        last = mid;
    }
    if(lcp<k){
        if(last<mid){
          b = mid-1;
        }
        else{
          a = mid+1;
        }
    }
    else {
      while(j+k<n && pos[mid]+k<m-1 && s1[j+k]==s2[pos[mid]+k]){
        suffix = mid;
        ++k;
      }
      if(  j+k<n &&  pos[mid]+k < m-1 && s1[j+k] < s2[pos[mid]+k]){
            b = mid-1;
      }
      else{
            a = mid + 1;
      }
      last = mid;
    }
    mid1 = mid;
    mid = a + (b-a)/2;
  }

  hint = mid;
  k1 = k;
  return k;

}

void rmqInit(std::vector<int>& v, std::vector<std::vector<int>>& u,std::vector<int>& w){
  int n = v.size();
  u = std::vector<std::vector<int>>(n,std::vector<int>(1,0));
  for(int i=0; i<n; ++i){
    u[i][0] = v[i];
  }
  w = std::vector<int>(n,0);
  int m(1);
  int k(2);
  for(int x=1; k<=n; ++x,k*=2){
    for(int i=m; i<m+(k>>1); ++i){
      w[i] = x-1;
    }
    m += (k>>1);
    for(int j=0; j<=n-k; ++j){
      u[j].push_back(std::min(u[j][x-1],u[j+(k>>1)][x-1]));
    }
  }
}

int rmqQuery(std::vector<std::vector<int>>& u, std::vector<int>& w, int i, int j){
  int k = w[j-i];
  return std::min(u[i][k],u[j-(1<<k)+1][k]);
}

void lcp(string& s, std::vector<int>& v, std::vector<int>& h){
  int n = v.size();
  std::vector<int> pos(n,0);
  for(int i=0; i<n; ++i){
    pos[v[i]] = i;
  }

  int k(0);
  for(int i=0; i<n-1; ++i){ // At most 2n char comparisons.
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
  u.push_back(-1);

  if(n<=2){
        std::iota(v.begin(),v.end(),1);
        v.push_back(0);
        if(n==2 && u[1]<=u[0]){
            std::swap(v[0],v[1]);
        }
    return;
  }

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

  // radix sort blocks of 3. O(n+|Σ|).
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

  // radix sort element at indices multiples of 3. O(n+|Σ|).
    std::vector<std::array<int,3>> u2(n-n1-n2,std::array<int,3>());
    for(int i=0; i<n-n1-n2; ++i){
        u2[i] = {{u[3*i],3*i+1<n?v2[i]:-1,3*i}};
    }
    std::vector<std::array<int,3>> u3(n-n1-n2,std::array<int,3>());
  mxm  = std::max(mxm,n1+n2);          //  |Σ| = size of current alphabet. (mxm ⩽ n && n1+n2 ⫹ 2n/3) ⇒ |Σ|⩽ n.

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

  // merge. O(n).
    int i1(0), i12(0);
  std::vector<int> v1(n1+n2,0);
  for(int i=0; i<n1; ++i){
    v1[v2[i]-1] = 3*(i) + 1;
  }
  for(int i=0; i<n2; ++i){
    v1[v2[n1+i]-1] = 3*(i) + 2;
  }

    for(int i=0; i<n; ++i){

    if(i1==n-n1-n2){
       v[v1[i12++]] = i;
    }
    else if(i12==n1+n2 || u[u2[i1][2]]<u[v1[i12]] ||
        (u[u2[i1][2]] == u[v1[i12]] && v1[i12]%3==1 && (u2[i1][2]+1==n || (v1[i12]+1<n && v2[u2[i1][2]/3] < v2[n1+v1[i12]/3])) ) ||
         (u[u2[i1][2]] == u[v1[i12]] && v1[i12]%3==2 && (u[u2[i1][2]+1] < u[v1[i12]+1]||  (u[u2[i1][2]+1] == u[v1[i12]+1] && (u2[i1][2]+2 == n || (v1[i12]+2<n && v2[u2[i1][2]/3+n1]<v2[v1[i12]/3+1])))) ) ){

        v[u2[i1++][2]] = i;
    }
    else{
        v[v1[i12++]] = i;
    }
    }

}
