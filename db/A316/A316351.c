/*
 * A316351  Numbers k such that k^2 + 1 has exactly four distinct prime
 *          factors.
 * https://oeis.org/A316351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A316351:
   numbers k such that k^2+1 has exactly four distinct prime factors.
   The function searches values of k starting from 1, tests the
   condition with count_distinct_primes, and memoises found terms. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t current_k = 0;

    while (seq_len < (size_t)n) {
        current_k++;                            /* try next k */
        uint64_t val = current_k * current_k + 1ULL;
        if (count_distinct_primes(val) == 4) {
            /* store found term */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (tmp == NULL) { /* allocation failure -> abort */
                    abort();
                }
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)current_k;
        }
    }
    return seq[n - 1];
}
