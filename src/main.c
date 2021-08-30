/*
 * Copyright (c) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/arch_interface.h>
#include <tinycrypt/sha256.h>
#include <tinycrypt/constants.h>
#include <kernel_internal.h>
#include <random/rand32.h>
#include <fcntl.h>
#include <posix/unistd.h>

#define FATFS_MNTP	"/RAM:"
#define TEST_DIR_FILE	FATFS_MNTP"/testfile.txt"

int file;


#define SIZE 50
const uint8_t rand[SIZE];


void random_gen(void *p1, void *p2, void *p3){

    sys_rand_get(rand, SIZE);
}

void sha256(void *p1, void *p2, void *p3){
    
    uint8_t digest[SIZE];
    struct tc_sha256_state_struct s;
    
    (void)tc_sha256_init(&s);
	tc_sha256_update(&s, (const uint8_t *)rand, SIZE);
	(void)tc_sha256_final(digest, &s);
}

void write_file(void *p1, void *p2, void *p3){

    open(TEST_DIR_FILE, O_CREAT | O_RDWR);
    lseek(file, 0, SEEK_SET);
    write(file, (char *)test_str, strlen(test_str));
    close(file);

}

void read_file(void *p1, void *p2, void *p3){
    
    char read_buff[80];
	size_t sz = strlen(test_str);
    open(TEST_DIR_FILE, O_CREAT | O_READ);
    read(file, read_buff, sz);
    close(file);

}

