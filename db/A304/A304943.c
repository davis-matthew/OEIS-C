/*
 * A304943  Number of ways to write n as the sum of a positive tribonacci
 *          number (A000073) and a positive odd squarefree number.
 * https://oeis.org/A304943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 868, u16 n <= 9747, u32 n <= 9747, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A304943: number of ways to write n as a positive tribonacci number */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* static state: computed tribonacci numbers up to the last one >= n */
    static bool initialized = false;
    static idx_t *trib = NULL;
    static size_t trib_len = 0;
    static size_t trib_cap = 0;

    /* initialise trib sequence: 1, 1, 2 */
    if (!initialized) {
        trib_cap = 8;
        trib = (idx_t *)malloc(trib_cap * sizeof(idx_t));
        trib[0] = (idx_t)1;
        trib[1] = (idx_t)1;
        trib[2] = (idx_t)2;
        trib_len = 3;
        initialized = true;
    }

    /* extend the trib list until the last value is >= n */
    while (trib_len > 0 && trib[trib_len - 1] < n) {
        if (trib_len == trib_cap) {
            trib_cap <<= 1;
            trib = (idx_t *)realloc(trib, trib_cap * sizeof(idx_t));
        }
        idx_t next = trib[trib_len - 1] +
                     trib[trib_len - 2] +
                     trib[trib_len - 3];
        trib[trib_len++] = next;
    }

    seq_t count = 0;

    /* count representations t + q = n, with t tribonacci, q odd squarefree */
    for (size_t i = 0; i < trib_len && trib[i] < n; ++i) {
        /* skip duplicate tribonacci values (e.g. the two initial 1's) */
        if (i > 0 && trib[i] == trib[i - 1]) continue;

        idx_t cand = n - trib[i];
        if ((cand & (idx_t)1) == 0) continue;            /* even → reject */
        if (is_squarefree((seq_t)cand)) ++count;         /* odd & squarefree */
    }

    return count;
}
