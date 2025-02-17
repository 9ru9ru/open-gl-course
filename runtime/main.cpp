#include "base/IBaseRunner.h"
#include "section01/Section01Runner.h"
#include "section02/FirstTriangleRunner.h"
#include "section02/TriangleTranslationRunner.h"
#include "section02/TriangleRotationRunner.h"
#include "section02/TriangleScalingRunner.h"
#include "section02/InterpolationRunner.h"
#include "section02/IndexedDrawRunner.h"

int main()
{
    // 해당 부분만 계속 변경하며 스터디.
    IBaseRunner* runner = new IndexedDrawRunner();
    runner->Run();
}