#pragma once

#include <QtGlobal>

/**
  Тестовый Enum, используется для демонстрации работы с enum-типами
*/
enum class InsertMode : quint32
{
    Unknown = 0,
    Single  = 1,
    Multi   = 2,
    Thread1 = 3,
    Thread2 = 4
};
