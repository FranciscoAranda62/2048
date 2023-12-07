#ifndef STORAGEDATA_H
#define SOTRAGEDATA_H

// NOTE: Storage positions must start with 0, directly related to file memory layout
typedef enum {
    STORAGE_POSITION_SCORE      = 0,
    STORAGE_POSITION_HISCORE    = 1
} StorageData;

bool SaveStorageValue(unsigned int position, int value);

int LoadStorageValue(unsigned int position);

#endif