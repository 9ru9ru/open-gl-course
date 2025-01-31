//
// Created by ypp06 on 2025-01-31.
//

#ifndef IBASERUNNER_H
#define IBASERUNNER_H



class IBaseRunner {
    public:
        virtual bool Run() = 0;
        virtual ~IBaseRunner() = default;
};



#endif //IBASERUNNER_H
