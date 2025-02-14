#include "base/IBaseRunner.h"
#include "section01/Section01Runner.h"
#include "section02/FirstTriangleRunner.h"
#include "section02/TriangleTranslationRunner.h"

int main()
{
    // 해당 부분만 계속 변경하며 스터디.
    IBaseRunner* runner = new TriangleTranslationRunner();
    runner->Run();
}