#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

const int INF = INT_MAX;
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, L; cin >> n >> m >> L;
    vector<vector<int>> stations_to_lines(n);
    vector<bool> adj(n,false), line_used(L,false);

    for (int i=0;i<m;i++){
        int x,y,l; cin>>x>>y>>l; x--;y--;l--;
        stations_to_lines[x].push_back(l);
        stations_to_lines[y].push_back(l);
        adj[x]=adj[y]=true; line_used[l]=true;
    }

    for (int i=0;i<n;i++) if(!adj[i]){cout<<-1<<"\n";return 0;}

    for (int i=0;i<n;i++){
        sort(stations_to_lines[i].begin(),stations_to_lines[i].end());
        stations_to_lines[i].erase(unique(stations_to_lines[i].begin(),stations_to_lines[i].end()),stations_to_lines[i].end());
    }

    vector<vector<int>> lines_to_stations(L);
    for (int s=0;s<n;s++){
        for (auto l:stations_to_lines[s]) if(line_used[l]) lines_to_stations[l].push_back(s);
    }

    for (int i=0;i<L;i++) if(line_used[i]){
        auto &v=lines_to_stations[i];
        sort(v.begin(),v.end());
        v.erase(unique(v.begin(),v.end()),v.end());
    }

    auto is_subset=[&](const vector<int>&A,const vector<int>&B){
        size_t i=0,j=0;
        while(i<A.size()&&j<B.size()){
            if(A[i]==B[j]){i++;j++;}
            else if(A[i]>B[j]) j++; else return false;
        }
        return i==A.size();
    };

    for (int i=0;i<L;i++){
        if(!line_used[i]) continue;
        for (int j=0;j<L;j++){
            if(i==j||!line_used[j]) continue;
            auto &Li=lines_to_stations[i], &Lj=lines_to_stations[j];
            if(Li.size()<=Lj.size()&&is_subset(Li,Lj)){line_used[i]=false;break;}
            if(Lj.size()<=Li.size()&&is_subset(Lj,Li)) line_used[j]=false;
        }
    }

    vector<vector<int>> line_adj(L);
    for (int i=0;i<n;i++){
        auto &Lns=stations_to_lines[i];
        for (size_t a=0;a<Lns.size();a++){
            int l1=Lns[a]; if(!line_used[l1]) continue;
            for (size_t b=a+1;b<Lns.size();b++){
                int l2=Lns[b]; if(!line_used[l2]) continue;
                line_adj[l1].push_back(l2);
                line_adj[l2].push_back(l1);
            }
        }
    }

    for (int i=0;i<L;i++) if(line_used[i]){
        sort(line_adj[i].begin(),line_adj[i].end());
        line_adj[i].erase(unique(line_adj[i].begin(),line_adj[i].end()),line_adj[i].end());
    }

    int ans=0;
    for (int i=0;i<L;i++){
        if(!line_used[i]) continue;
        vector<int> dist(L,INF); dist[i]=0;
        queue<int>q;q.push(i);
        while(!q.empty()){
            int u=q.front();q.pop();
            for (auto v:line_adj[u]){
                if(line_used[v]&&dist[v]>dist[u]+1){
                    dist[v]=dist[u]+1; q.push(v);
                }
            }
        }
        for (int j=0;j<L;j++) if(line_used[j]) ans=max(ans,dist[j]);
        if(ans==INF){cout<<-1<<"\n";return 0;}
    }

    cout<<ans<<"\n";
}