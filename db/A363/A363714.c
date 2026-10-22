/*
 * A363714  Numbers that are the sum of a positive square and a positive
 *          fourth power in more than one way.
 * https://oeis.org/A363714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_cnt, alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence by enumerating all pairs
   (x, y) of positive integers with x^2 + y^4 ≤ N and counting the
   number of representations.  All numbers having more than one
   representation are stored in ascending order. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_cnt.h"
#include "../utils/alloc_seq.h"

/* The types seq_t and idx_t are defined by the build system. */

/* Static state used for memoization */
static seq_t *results   = NULL;   /* sorted list of terms */
static size_t res_len   = 0;      /* number of terms currently stored */
static size_t res_cap   = 0;      /* current capacity of results array */
static size_t cur_limit = 0;      /* maximum value that has been checked */

/* Allocate or resize a seq_t array with an explicit cast for C++ */

/* Build all representations up to new_limit and rebuild the
   result list.  new_limit must be ≥ 1. */
static void build_up_to(size_t new_limit)
{
    /* Counter for number of representations of each sum. */
    uint32_t *cnt = alloc_cnt(new_limit + 1);
    if (cnt == NULL)
        exit(EXIT_FAILURE);

    /* Enumerate all pairs (x, y).  Use unsigned __int128 for safety. */
    for (unsigned long long y = 1; ; ++y) {
        unsigned __int128 y4 = (unsigned __int128)y * y * y * y;
        if (y4 > new_limit) break;
        for (unsigned long long x = 1; ; ++x) {
            unsigned __int128 x2  = (unsigned __int128)x * x;
            unsigned __int128 sum = y4 + x2;
            if (sum > new_limit) break;
            ++cnt[(size_t)sum];
        }
    }

    /* Rebuild the sorted list of numbers with >1 representation. */
    seq_t *new_res   = NULL;
    size_t new_cap   = 0;
    size_t new_len   = 0;

    for (size_t i = 1; i <= new_limit; ++i) {
        if (cnt[i] > 1) {
            if (new_len == new_cap) {
                new_cap = (new_cap == 0) ? 16 : new_cap * 2;
                seq_t *tmp = (seq_t *)realloc(new_res, new_cap * sizeof(seq_t));
                if (tmp == NULL)
                    exit(EXIT_FAILURE);
                new_res = tmp;
            }
            new_res[new_len++] = (seq_t)i;
        }
    }

    free(cnt);

    /* Replace the old result array with the new one. */
    free(results);
    results   = new_res;
    res_len   = new_len;
    res_cap   = new_cap;
    cur_limit = new_limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */

    /* Ensure we have at least one prebuilt list. */
    if (results == NULL)
        build_up_to(100000);        /* initial guess; can be increased */

    /* Grow until enough terms are available. */
    while (res_len < (size_t)n)
        build_up_to(cur_limit * 2); /* double the limit each time */

    return results[n - 1]; /* a(1) is the first element */
}
