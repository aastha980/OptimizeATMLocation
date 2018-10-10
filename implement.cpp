#include<bits/stdc++.h>
using namespace std;

#define mutationRate 10
#define crossoverRate 50
#define maxLocation 450
#define maxCost 100000000
struct node{
    string location;
    string population;
    string proximity;
    string cost;
};

float fitness(vector<node> chromosome, vector<int> chosen){
    int pop=0,pro=0,cos=0;

    for(int i=0;i<chromosome.size();i++){
        //cout<<chromosome[i]->population<<" "<<chosen[i]<<endl;
        int thevalue;
        istringstream ss(chromosome[i].population);
        ss >> thevalue;
        pop+=chosen[i]*thevalue;
    }
    for(int i=0;i<chromosome.size();i++){
        int thevalue;
        istringstream ss(chromosome[i].proximity);
        ss >> thevalue;
        pro+=chosen[i]*thevalue;
    }
    for(int i=0;i<chromosome.size();i++){
        int thevalue;
        istringstream ss(chromosome[i].cost);
        ss >> thevalue;
        cos+=chosen[i]*thevalue;
    }
    float fitnessval=0.010*pop+0.001*pro/0.015+0.020*cos;
    return fitnessval*0.001;
}

bool myfun(pair<vector<int>,float> a1,pair<vector<int>,float> a2){
    return a1.second<a2.second;
}

bool myfun2(pair<vector<int>,float> a1,pair<vector<int>,float> a2){
    return a1.second==a2.second;
}

vector<pair<vector<int>,float> > roulette(vector<pair<vector<int>,float> > group,int num){
    int totalfitess=0;
    vector<pair<vector<int>,float> > roulettevector;
    for(int i=0;i<group.size();i++){
        totalfitess+=group[i].second;
    }
    //cout<<totalfitess<<endl;
    for(int i=0;i<num;i++){
        int j=rand()%totalfitess;
        //cout<<j<<endl;
        int ans=0,pointer=0;
        while(ans<j){
            ans+=group[pointer].second;
            pointer++;
        }
        //cout<<pointer<<endl;
        roulettevector.push_back(group[pointer]);
    }
    sort(roulettevector.begin(),roulettevector.end(),myfun);
    roulettevector.resize(distance(roulettevector.begin(), unique(roulettevector.begin(),roulettevector.end(),myfun2)));
    return roulettevector;
}

int main(){
    ifstream file ( "MOCK_DATA.csv" );
    string value;
    vector<node> data;
    int p=0,l=0;
    while ( getline ( file, value, '\n' ) ){
        if(p==0){
            p++;
            continue;
        }
        string r= string( value, 0, value.length() );
        //cout<<r<<endl;
        int sizearray=r.length()+1;
        char str[sizearray];
        strcpy(str,r.c_str());
        char *token = strtok(str, ",");

        vector<string> v;
        while (token != NULL) {
            v.push_back(token);
            token = strtok(NULL, ",");
        }
        //cout<<v[3]<<endl;
        node n;
        n.location = v[0];
        n.population = v[1];
        n.proximity = v[2];
        n.cost = v[3];
        data.push_back(n);
    }
    //cout<<data.size()<<endl;
    int t=60;
    vector<pair<vector<int>,float> > group;
    while(group.size()<t){
        //cout<<group.size()<<endl;
        vector<int> v;
        for(int i=0;i<data.size();i++){
            int j=rand()%2;
            v.push_back(j);
        }
        int costt=0,m=0;
        for(int i=0;i<data.size();i++){
            m+=v[i];
            int thevalue;
            istringstream ss(data[i].cost);
            ss >> thevalue;
            costt+=v[i]*thevalue;

        }
        if(costt<=maxCost&&m<=maxLocation){
        cout<<costt<<" "<<m<<endl;
            group.push_back({v,0.0});
        }
    }
    unique(group.begin(),group.end());
//    for(int i=0;i<group.size();i++){
//        cout<<i<<" ";
//        for(int j=0;j<group[i].first.size();j++){
//            cout<<group[i].first[j]<<" ";
//        }
//        cout<<endl;
//    }
    for(int i=0;i<group.size();i++){
        group[i].second=fitness(data,group[i].first);
    }
    /*
        selection by roulette wheel
    */
    vector<pair<vector<int>,float> > rouletteChromosomes=roulette(group,50);
//    for(int i=0;i<rouletteChromosomes.size();i++){
//        cout<<i<<" "<<rouletteChromosomes[i].second;
//        cout<<endl;
//    }

    int generations=200,index=0;
    while(generations--){

        /*
            Crossover
        */
        while(index+1<rouletteChromosomes.size()){
            int j=rand()%101;
            if(j<crossoverRate){
                vector<int> v1,v2;
                v1=rouletteChromosomes[index].first;
                v2=rouletteChromosomes[index+1].first;
                float f1=rouletteChromosomes[index].second;
                float f2=rouletteChromosomes[index+1].second;
                int randomNum=rand()%rouletteChromosomes[index].first.size()+1;
                for(int k=randomNum;k<rouletteChromosomes[index].first.size();k++){
                    int temp=v1[k];
                    v1[k]=v2[k];
                    v2[k]=temp;
                }
                vector<float> l;
                l.push_back(f1);l.push_back(f2);
                int f3=fitness(data,v1);
                int f4=fitness(data,v2);
                int costt=0,m=0;
                for(int i=0;i<data.size();i++){
                    m+=v1[i];
                    int thevalue;
                    istringstream ss(data[i].cost);
                    ss >> thevalue;
                    costt+=v1[i]*thevalue;

                }
                //cout<<costt<<" "<<m<<endl;
                if(costt<=maxCost&&m<=maxLocation){
                    l.push_back(f3);
                }
                costt=0;m=0;
                for(int i=0;i<data.size();i++){
                    m+=v2[i];
                    int thevalue;
                    istringstream ss(data[i].cost);
                    ss >> thevalue;
                    costt+=v2[i]*thevalue;

                }
                //cout<<costt<<" "<<m<<endl;
                if(costt<=maxCost&&m<=maxLocation){
                    l.push_back(f4);
                }
                sort(l.begin(),l.end());

                //cout<<f1<<" "<<f2<<" "<<f3<<" "<<f4<<endl;
                if(l[l.size()-1]==f3){
                    if(f1<f2){
                        rouletteChromosomes[index].first=v1;
                        rouletteChromosomes[index].second=f3;
                    }
                    else{
                        rouletteChromosomes[index+1].first=v1;
                        rouletteChromosomes[index+1].second=f3;
                    }
                }else if(l[l.size()-1]==f4){
                    if(f1<f2){
                        rouletteChromosomes[index].first=v2;
                        rouletteChromosomes[index].second=f4;
                    }
                    else{
                        rouletteChromosomes[index+1].first=v2;
                        rouletteChromosomes[index+1].second=f4;
                    }
                }
                if(l[l.size()-2]==f3){
                    if(f1<f2){
                        rouletteChromosomes[index].first=v1;
                        rouletteChromosomes[index].second=f3;
                    }
                    else{
                        rouletteChromosomes[index+1].first=v1;
                        rouletteChromosomes[index+1].second=f3;
                    }
                }else if(l[l.size()-2]==f4){
                    if(f1<f2){
                        rouletteChromosomes[index].first=v2;
                        rouletteChromosomes[index].second=f4;
                    }
                    else{
                        rouletteChromosomes[index+1].first=v2;
                        rouletteChromosomes[index+1].second=f4;
                    }
                }
            }
            index+=2;
        }
        sort(rouletteChromosomes.begin(),rouletteChromosomes.end(),myfun);

        /*
            mutation
        */
        int k=ceil((float)mutationRate/100*rouletteChromosomes.size());
        //cout<<k<<endl;
        int jj,hh;
        while(k--){
            jj=rand()%rouletteChromosomes.size();
            hh=rand()%rouletteChromosomes[jj].first.size();
            rouletteChromosomes[jj].first[hh]=1-rouletteChromosomes[jj].first[hh];
            rouletteChromosomes[jj].second=fitness(data,rouletteChromosomes[jj].first);
        }
    }
    for(int i=0;i<rouletteChromosomes[rouletteChromosomes.size()-1].first.size();i++){
        cout<<rouletteChromosomes[rouletteChromosomes.size()-1].first[i]<<" ";
    }
    cout<<endl;
    cout<<rouletteChromosomes[rouletteChromosomes.size()-1].second<<endl;
}
