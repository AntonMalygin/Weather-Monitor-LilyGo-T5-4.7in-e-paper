// MIT License (MIT) (C) 2022 gembeider@ya.ru (https://gitflic.ru/user/gembeider)

#pragma once

#include "Rect.h"

class Table : public Rect {
protected:
  int _ColCount = 0;
  int _RowCount = 0;
  int * _ColSizes = NULL;
  int * _RowSizes = NULL;
public:
  Table(int x = 0, int y = 0, int w = 0, int h = 0): Rect(x, y, w, h) {}
  int ColCount() const {return _ColCount;}
  int RowCount() const {return _RowCount;}

  void setCols(int count, const int * sizes = NULL) {
    _ColCount = count;
    if (_ColSizes)
      delete [] _ColSizes;
    _ColSizes = new int [count];
    if (sizes) {
    	for (int i = 0; i < count; ++i){
    	  _ColSizes[i]	= sizes[i];
    	}
    } else {
    	int w = Width;
      int c = w /count;
      int s = w - c * count;
      for (int i = 0; i < count; ++i) {
        _ColSizes[i] = c;
        if (s > 0) {
          ++_ColSizes[i];
          --s;
        }
      }
    }
  } // setCols();

  void setRows(int count, const int * sizes = NULL) {
    _RowCount = count;
    if (_RowSizes)
      delete [] _RowSizes;
    _RowSizes = new int [count];
    if (sizes) {
    	for (int i = 0; i < count; ++i){
    	  _RowSizes[i]	= sizes[i];
    	}
    } else {
    	int w = Width;
      int c = w /count;
      int s = w - c * count;
      for (int i = 0; i < count; ++i) {
        _RowSizes[i] = c;
        if (s > 0) {
          ++_RowSizes[i];
          --s;
        }
      }
    }
  } // setRows

  bool getCellRect(int col, int row, Rect &rect){
   if ((col >= 0) && (col < _ColCount) && (row >= 0) && (row < _RowCount)) {
      rect.Left = Left;
      for (int i = 0; i < col; ++i) {
        rect.Left += _ColSizes[i];
      }
      rect.Width = _ColSizes[col];
      rect.Top = Top;
      for (int i = 0; i < row; ++i) {
        rect.Top += _RowSizes[i];
      }
      rect.Height = _RowSizes[row];
      return true;
    } else
    return false;
  }

  Rect getCellRect(int col, int row) {
  	Rect rect;
    getCellRect(col, row, rect);
    return rect;
  }
};
