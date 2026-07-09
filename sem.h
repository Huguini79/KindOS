#ifndef SEM_IPC_H
#define SEM_IPC_H

struct sem
{
    ushort semval;
    U32 sempid;
    ushort semncnt;
    ushort semzcnt;
};

struct semid_ds
{
    U8 flags;
    struct sem* sem_base;
    ushort sem_nsems;
};

struct sembuf
{
    unsigned short sem_num;
    short sem_op;
    short sem_flg;
};

struct semid_ds sems[256];

#endif