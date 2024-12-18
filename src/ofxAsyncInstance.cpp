#include "ofxAsyncInstance.h"

ofxAsyncInstance::ofxAsyncInstance(){
    thread_id_max = 0;
    wait_for_all_when_exit = true;
}

ofxAsyncInstance::~ofxAsyncInstance(){
    if (wait_for_all_when_exit) {
        try{
            stopAll(true);
        }catch(...){}
    }else{
        try{
            stopAll(false);
        }catch(...){}
    }
}

int ofxAsyncInstance::run(std::function<void()> func){
    int runner_id = ++thread_id_max;
    auto runner = make_shared<ofxAsyncInstance::AsyncRunner>();
    runners[runner_id] = runner;
    runner->setup(func);
    runner->startThread();
    return runner_id;
}

int ofxAsyncInstance::run(std::function<void(ofThread*)> func){
    int runner_id = ++thread_id_max;
    auto runner = make_shared<ofxAsyncInstance::AsyncRunnerWithArg>();
    runners[runner_id] = runner;
    runner->setup(func);
    runner->startThread();
    return runner_id;
}

void ofxAsyncInstance::update(){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners[key];
        if(!e || !e->isThreadRunning()){
            runners.erase(key);
            return;
        }
    }
}

bool ofxAsyncInstance::exists(int thread_id){
    return runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning();
}

bool ofxAsyncInstance::isRunning(int thread_id){
    return runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning();
}

void ofxAsyncInstance::stop(int thread_id, bool wait_until_stop){
    if(runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning()){
        auto e = runners[thread_id];
        e->stopThread();
        if(wait_until_stop){
            e->waitForThread(false);
        }
    }

    runners.erase(thread_id);
}

void ofxAsyncInstance::stopAll(bool wait_until_stop){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners[key];
        if(e->isThreadRunning()){
            e->stopThread();
            
            if(wait_until_stop){
                e->waitForThread(false);
            }
        }
    }
    
    runners.clear();
}

void ofxAsyncInstance::waitFor(int thread_id){
    if(runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning()){
        auto e = runners[thread_id];
        e->waitForThread(false);
    }

    runners.erase(thread_id);
}

void ofxAsyncInstance::waitForAll(){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners[key];
        if(e->isThreadRunning()){
            e->waitForThread(false);
        }
    }

    runners.clear();
}

shared_ptr<ofThread> ofxAsyncInstance::getThread(int thread_id){
    if(runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning()){
        auto e = runners[thread_id];
        return e;
    }else{
        return nullptr;
    }
}

void ofxAsyncInstance::setExitAction(bool wait_for_all){
    wait_for_all_when_exit = wait_for_all;
}