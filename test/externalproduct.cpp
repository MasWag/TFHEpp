#include<tfhe++.hpp>
#include<cassert>
#include<random>
#include<iostream>

using namespace std;
using namespace TFHEpp;

int main(){
    const uint32_t num_test = 100;
    random_device seed_gen;
    default_random_engine engine(seed_gen());
    uniform_int_distribution<uint32_t> binary(0, 1);

    cout<<"test p=1"<<endl;
    for(int test;test<num_test;test++){
        lweKey key;

        array<bool,DEF_N> p;
        for(bool &i : p) i = (binary(engine)>0);
        array<uint32_t,DEF_N> pmu;
        for(int i = 0; i<DEF_N; i++ ) pmu[i] = p[i]?DEF_MU:-DEF_MU;
        array<array<uint32_t,DEF_N>,2> c = trlweSymEncryptlvl1(pmu,DEF_αbk,key.lvl1);

        array<array<array<double,DEF_N>,2>,2*DEF_l> trgswfft = trgswfftSymEncryptlvl1(1,DEF_αbk,key.lvl1);
        array<array<uint32_t,DEF_N>,2> res;
        trgswfftExternalProductlvl1(res,trgswfft,c);
        array<bool,DEF_N> p2 = trlweSymDecryptlvl1(res,key.lvl1);
        for(int i = 0;i<DEF_N;i++) assert(p[i] == p2[i]);
    }
    cout<<"Passed"<<endl;

    cout<<"test p=-1"<<endl;
    for(int test;test<num_test;test++){
        lweKey key;

        array<bool,DEF_N> p;
        for(bool &i : p) i = binary(engine) > 0;
        array<uint32_t,DEF_N> pmu;
        for(int i = 0; i<DEF_N; i++ ) pmu[i] = p[i]?DEF_MU:-DEF_MU;
        array<array<uint32_t,DEF_N>,2> c = trlweSymEncryptlvl1(pmu,DEF_αbk,key.lvl1);

        array<array<array<double,DEF_N>,2>,2*DEF_l> trgswfft = trgswfftSymEncryptlvl1(-1,DEF_αbk,key.lvl1);
        array<array<uint32_t,DEF_N>,2> res;
        trgswfftExternalProductlvl1(res,trgswfft,c);
        array<bool,DEF_N> p2 = trlweSymDecryptlvl1(res,key.lvl1);
        for(int i = 0;i<DEF_N;i++) assert(p[i] == !p2[i]);
    }
    cout<<"Passed"<<endl;
}