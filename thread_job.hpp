#pragma once
class thread_job {
public:
    virtual void operator() () = 0;
};