#include "Config.h"

int Config::getheight()
{
    return this->height;
}

int Config::getwidth()
{
    return this->width;
}

int Config::getlinesize()
{
    return this->linesize;
}

bool Config::getfirstwin()
{
    return this->firstwin;
}

int Config::getMaxLinesNum()
{
    return this->maxLines;
}

Config::Config(int newheight, int newwidth, int newMaxLines, int newlinesize)
{
    this->height = newheight;
    this->width = newwidth;
    this->linesize = newlinesize;
    this->maxLines = newMaxLines;
}

Config::Config()
{
}
