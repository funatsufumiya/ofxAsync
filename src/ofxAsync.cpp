#include "ofxAsync.h"

std::map<int, shared_ptr<ofxAsync::AsyncRunner> > ofxAsync::runners = {};
ofxAsync::AsyncExit ofxAsync::asyncExit = ofxAsync::AsyncExit();

int ofxAsync::run(std::function<void()> func){
    int runner_id = runners.size();
    runners[runner_id] = make_shared<AsyncRunner>();
    
    auto& runner = runners[runner_id];
    runner->setup(func);
    runner->startThread();
    
    return runner_id;
}

void ofxAsync::update(){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners[key];
        if(!e || !e->isThreadRunning()){
            runners.erase(key);
            return;
        }
    }
}

void ofxAsync::stop(int thread_id){
    
}

void ofxAsync::stopAll(){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners[key];
        if(e->isThreadRunning()){
            e->stopThread();
            e->waitForThread(false);
        }
    }
    
    while(true){
        if( runners.size() == 0 ){
            break;
        }else{
            auto& key = runners.begin()->first;
            runners.erase(key);
        }
    }
}
