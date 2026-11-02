#pragma once;


class Concert {
public:
    Concert() {}
    virtual ~Concert() {}
    virtual void play() const = 0;
};