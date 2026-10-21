/*
 * A265113  Primes p such that p and p^2 have the same number of 1's in their
 *          binary representations.
 * https://oeis.org/A265113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 116, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes p such that popcount(p) = popcount(p^2) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static inline int popcnt_seq(seq_t x)
{
    int cnt = 0;
    while (x) {
        x &= x - 1; /* clear lowest set bit */
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static seq_t *terms = NULL;
    static idx_t term_count = 0;
    static idx_t term_cap   = 0;
    static uint64_t current_prime = 1;  /* start just before the first prime */

    while (term_count < n) {
        uint64_t p = next_prime(current_prime);
        current_prime = p;

        seq_t sp  = (seq_t)p;
        seq_t sp2 = sp * sp;

        if (popcnt_seq(sp) == popcnt_seq(sp2)) {
            if (term_count >= term_cap) {
                term_cap = (term_cap ? term_cap * 2 : 64);
                terms = (seq_t*)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_count++] = sp;
        }
    }
    return terms[n - 1];
}
