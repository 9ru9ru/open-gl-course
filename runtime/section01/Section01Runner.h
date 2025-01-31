//
// Created by ypp06 on 2025-01-31.
//

#ifndef SECTION01RUNNER_H
#define SECTION01RUNNER_H

#include "../base/IBaseRunner.h"
#include <GL/glew.h>

class Section01Runner : public IBaseRunner
{
    public:
        bool Run() override;
        ~Section01Runner() override = default;
    private:
        const GLint windowWidth = 800, windowHeight = 600;
};



#endif //SECTION01RUNNER_H
