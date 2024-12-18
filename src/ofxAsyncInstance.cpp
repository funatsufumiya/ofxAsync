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
    gc();

    int runner_id = ++thread_id_max;
    auto runner = make_shared<ofxAsyncInstance::AsyncRunner>();
    runners[runner_id] = runner;
    runner->setup(func);
    runner->startThread();
    return runner_id;
}

int ofxAsyncInstance::run(std::function<void(ofThread*)> func){
    gc();
    
    int runner_id = ++thread_id_max;
    auto runner = make_shared<ofxAsyncInstance::AsyncRunnerWithArg>();
    runners[runner_id] = runner;
    runner->setup(func);
    runner->startThread();
    return runner_id;
}

void ofxAsyncInstance::gc(){
    auto iter = runners.begin();
    auto endIter = runners.end();

    for(; iter != endIter; ) {
        if(!iter->second || !iter->second->isThreadRunning()){
            iter = runners.erase(iter);
        }else{
            ++iter;
        }
    }
}

void ofxAsyncInstance::update(){
    // do nothing, exists just for compatibility
}

bool ofxAsyncInstance::exists(int thread_id){
    return runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning();
}

bool ofxAsyncInstance::isRunning(int thread_id){
    return runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning();
}

void ofxAsyncInstance::stop(int thread_id, bool wait_until_stop){
    if(runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning()){
        auto e = runners.at(thread_id);
        if(e){
            e->stopThread();
            if(wait_until_stop){
                e->waitForThread(false);
            }
        }
    }

    runners.erase(thread_id);
    gc();
}

void ofxAsyncInstance::stopAll(bool wait_until_stop){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners.at(key);
        if(e && e->isThreadRunning()){
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
        auto e = runners.at(thread_id);
        e->waitForThread(false);
    }

    runners.erase(thread_id);
    gc();
}

void ofxAsyncInstance::waitForAll(){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners.at(key);
        if(e && e->isThreadRunning()){
            e->waitForThread(false);
        }
    }

    runners.clear();
}

shared_ptr<ofThread> ofxAsyncInstance::getThread(int thread_id){
    if(runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning()){
        auto e = runners.at(thread_id);
        return e;
    }else{
        return nullptr;
    }
}

void ofxAsyncInstance::setExitAction(bool wait_for_all){
    wait_for_all_when_exit = wait_for_all;
}
