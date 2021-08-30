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
#include <string.h>
#include <fcntl.h>
#include <posix/unistd.h>
#include <stdio.h>
#include <sys/printk.h>

#define NUM_THREADS 4


#define TEST_DIR_FILE	"/home/alisson/zephyrproject/zephyr/projeto_sistemas_embarcados/src/testfile.txt"

#define SIZE 50

#define STACK_SIZE (1024 + CONFIG_TEST_EXTRA_STACKSIZE)

int file;



uint8_t rand[SIZE];

const uint8_t write_buffer[SIZE];



K_THREAD_STACK_EXTERN(tstack);
K_THREAD_STACK_ARRAY_DEFINE(tstacks, NUM_THREADS, STACK_SIZE);

static struct k_thread t[NUM_THREADS];



void random_gen(void *p1, void *p2, void *p3){
    
    sys_rand_get(rand, SIZE);
}

void sha256(void *p1, void *p2, void *p3){
    
    uint8_t digest[SIZE];
    struct tc_sha256_state_struct s;
    
    (void)tc_sha256_init(&s);
	tc_sha256_update(&s, (const uint8_t *)rand, SIZE);
	(void)tc_sha256_final(digest, &s);
	memcpy(write_buffer, digest, SIZE);
	printf("bb");
}

void write_file(void *p1, void *p2, void *p3){

    file = open(TEST_DIR_FILE, O_RDWR);
    lseek(file, 0, SEEK_SET);
    write(file, (char *)write_buffer, SIZE);
    close(file);
    printf("cc");

}

void read_file(void *p1, void *p2, void *p3){
    
    char read_buff[80];
	size_t sz = SIZE;
    read(file, read_buff, sz);
    close(file);
    printk("%d", read_buff[0]);

}

void main(void){
    
    	long t1 = 1, t2 = 2, t3 = 3, t4 = 4;
    
    k_thread_create(&t[0], tstacks[0], STACK_SIZE, random_gen,
			INT_TO_POINTER(t1), NULL, NULL, 0, 0,
			K_NO_WAIT);

	k_thread_create(&t[1], tstacks[1], STACK_SIZE, sha256,
			INT_TO_POINTER(t2), NULL, NULL, 1, 0,
			K_NO_WAIT);

	k_thread_create(&t[2], tstacks[2], STACK_SIZE, write_file,
			INT_TO_POINTER(t3), NULL, NULL, 2, 0,
			K_NO_WAIT);
			
    k_thread_create(&t[3], tstacks[3], STACK_SIZE, read_file,
			INT_TO_POINTER(t4), NULL, NULL, 3, 0,
			K_NO_WAIT);
    /* Wait for all threads to complete */
	for (int i = 0; i < NUM_THREADS; i++) {
		k_thread_join(&t[i], K_FOREVER);
	}

}
