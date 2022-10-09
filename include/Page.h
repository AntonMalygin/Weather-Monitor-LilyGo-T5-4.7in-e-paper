// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Rect.h"
#include "Button2m.h"


#define FRAME_LIST_SIZE  15
#define PAGE_LIST_SIZE   8

class Frame: public Rect  {
  public:
    boolean Visible = true;
    Frame(int x = 0, int y = 0, int w = 0, int h = 0): Rect(x, y, w, h) {
      ;
    }
    virtual void draw() {
    }
};
class FrameList {
  protected:
    Frame * _frames[FRAME_LIST_SIZE];
    const int16_t _size = FRAME_LIST_SIZE;
  public:
    void addFrame(Frame * frame) {
      for (int i = 0; i < _size; i++) {
        if (_frames[i] == frame)
          return;
      } //for
      for (int i = 0; i < _size; i++) {
        if (!_frames[i]) {
          _frames[i] = frame;
          return;
        } //if
      } //for
    };

    virtual void draw() {
      for (int i = 0; i < _size; i++) {
        if (_frames[i] != NULL) {
          if (_frames[i]->Visible) {
            _frames[i]->draw();
          } //if
        } //if
      } //for
    }; // draw()
};

class Page: public FrameList {
  enum PageEvents{
      PageActivete,
      PageDeactivate,
      PageSleep,
      PageWakeup,
      PageTimer
    };
  protected:
    void drawFrameList() {
      FrameList::draw();
    }
  public:
    String Id = "";
    int    Tag = 0;
    virtual void onButton(Button2m& btn, Button2m::Event_t event, uint16_t mask) {;};
    virtual void onEvent(Page::PageEvents event) {};
    void draw() {
      FrameList::draw();
    }
};

class PageList: public Page {
  private:
    Page * * _pages;
    int16_t _size = 0;
    int16_t _count = 0;
    int16_t _activeIndex = -1;
  public:
    PageList(int size): _size(size) {
      _pages = new Page * [size];
    };
    ~PageList() {
      delete [] _pages;
    };
    int16_t addPage(Page * page) {
      if (_count == _size)
        return -2;
      if (page == NULL)
        return -1;
      int16_t index = _count;
      _pages[index] = page;
      if (index == 0)
        _activeIndex = 0;
      _count++;
      return index;
    };
    Page * activePage() {
      if (checkIndex(_activeIndex))
        return _pages[_activeIndex];
      else
        return NULL;
    }
    bool checkIndex(int16_t index) {
      return ((index >= 0) && (index < _count));
    }
    int16_t activeIndex() {
      return _activeIndex;
    }
    int16_t activeIndex(int16_t index) {
      int16_t prevIndex = _activeIndex;
      if (checkIndex(index))
        _activeIndex = index;
      return prevIndex;
    }
    void draw() {
      FrameList::draw();
      Page * page = activePage();
      if (page != NULL)
        page->draw();
    }
    void nextPage() {
      if (++_activeIndex >= _count)
        _activeIndex = 0;
    }
    void prevPage() {
      if (--_activeIndex < 0)
        _activeIndex = 0;
    }
    int16_t count() {return _count; }
};
