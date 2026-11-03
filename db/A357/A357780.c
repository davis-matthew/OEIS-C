/*
 * A357780  Primes p such that changing, in p, all 1's to 2's we get
 *          semiprimes and changing all 1's to 3's we get triprimes.
 * https://oeis.org/A357780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 283, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 967 (2026-11-01)
 * Helpers:   bigomega_seq, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate over primes, substitute all decimal 1's with 2 and 3,
   and keep those whose substitutions have exactly 2 and 3 prime factors
   (with multiplicity).  Results are cached for future calls. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"
#include "../utils/next_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* replace all decimal digits 1 by the digit 'repl' (2 or 3). */
static seq_t substitute_digit(seq_t n, int repl)
{
    seq_t t = n;
    if (t <= 0) return (seq_t)repl;

    /* collect digits in reverse order */
    uint8_t dig[128];
    size_t len = 0;
    while (t > 0) {
        uint64_t d = (uint64_t)(t % 10);
        dig[len++] = (uint8_t)d;
        t = t / 10;
    }

    /* rebuild number from most significant to least significant digit */
    seq_t res = (seq_t)0;
    for (size_t i = len; i-- > 0; ) {
        uint8_t d = dig[i];
        if (d == 1) d = (uint8_t)repl;
        res = res * 10 + (seq_t)d;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* cached sequence values */
    static seq_t *list = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static uint64_t start = 2;      /* next candidate for next_prime */

    while (len < n) {
        /* obtain next prime candidate */
        uint64_t p64 = next_prime(start);
        start = p64 + 1;           /* advance search start */
        seq_t p = (seq_t)p64;       /* prime as seq_t */

        seq_t p2 = substitute_digit(p, 2);
        seq_t p3 = substitute_digit(p, 3);

        seq_t f2 = bigomega_seq(p2);
        seq_t f3 = bigomega_seq(p3);

        if (f2 == (seq_t)2 && f3 == (seq_t)3) {
            /* grow cache if necessary */
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = seq_realloc(list, cap, new_cap);
                if (!tmp) return (seq_t)0;   /* allocation failed */
                list = tmp;
                cap  = new_cap;
            }
            list[len++] = p;
        }
    }
    return list[n - 1];
}
