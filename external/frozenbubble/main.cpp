#include "main.hpp"
#include "FBApp.hpp"
#include "GlobalSetting.hpp"

int main(int argc, char *argv[])
{
    (void)argc;(void)argv;
    
    return App::Running(&GlobalSetting::Inst(), FBApp());
    
}


