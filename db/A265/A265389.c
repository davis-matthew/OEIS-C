/*
 * A265389  The sums from the following procedure: from the list of positive
 *          integers, repeatedly remove the first three numbers and their sum.
 * https://oeis.org/A265389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 6553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: compute successive sums obtained by repeatedly
   removing the first three numbers of the current list of
   positive integers together with their sum. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

/* static state shared among all calls */
static seq_t *sums = NULL;      /* array of computed sums (a(k)) */
static size_t sums_cap = 0;     /* current capacity of 'sums' */
static size_t sums_len = 0;     /* number of sums stored so far */
static idx_t cur = 1;           /* next integer to inspect in the list */
static size_t sum_idx = 0;      /* index into 'sums' used for skipping */

/* fetch the next available integer in the list
   (skipping any values that have already been removed as sums) */
static inline seq_t get_next(void) {
    /* skip any removed sum equal to cur */
    while (sum_idx < sums_len && sums[sum_idx] == (seq_t)cur) {
        sum_idx++;
        cur++;
    }
    seq_t val = (seq_t)cur;
    cur++;
    return val;
}

/* main function referred to by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0, but guard anyway */

    /* compute additional sums up to requested index */
    if ((size_t)n > sums_len) {
        for (size_t i = sums_len; i < (size_t)n; ++i) {
            seq_t x1 = get_next();
            seq_t x2 = get_next();
            seq_t x3 = get_next();
            seq_t sum = x1 + x2 + x3;

            /* grow storage if needed */
            if (sums_len == sums_cap) {
                size_t new_cap = sums_cap ? sums_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
                assert(tmp != NULL);      /* abort on allocation failure */
                sums = tmp;
                sums_cap = new_cap;
            }
            sums[sums_len++] = sum;
        }
    }
    return sums[n - 1];
}
