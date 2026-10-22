/*
 * A356431  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number not occurring earlier that shares a factor with both a(n-1)
 *          and the number of divisors of a(n-1).
 * https://oeis.org/A356431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A356431 (as defined in the OEIS entry).
   a(1)=1, a(2)=2.
   For n>2, a(n) is the smallest positive integer not yet used that shares
   a non‑trivial common factor with both a(n-1) and d(a(n-1)), where d(x)
   denotes the number of positive divisors of x. */

/* --- required standard headers --- */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
/* --- utility headers providing gcd and prime factorisation --- */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/factor_u64.h"   /* void factor_u64(...); */

/* --- helper to compute the divisor count of a uint64_t number --- */
static uint64_t divisor_count_u64(uint64_t n)
{
    if (n <= 1U)
        return 1U;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    uint64_t d = 1ULL;
    for (size_t i = 0; i < cnt; ++i)
        d *= exps[i] + 1ULL;
    return d;
}

/* --- main sequence function as requested --- */
seq_t a(idx_t n)
{
    /* static cache of already computed terms (1‑based indexing) */
    static seq_t *cache     = NULL; /* array of terms */
    static idx_t  length    = 0;    /* number of terms stored (== current size) */
    static size_t capacity  = 0;    /* allocated capacity */

    /* extend cache up to n */
    while (length < n) {
        seq_t term;

        if (length == 0) {               /* a(1) */
            term = (seq_t)1;
        } else if (length == 1) {         /* a(2) */
            term = (seq_t)2;
        } else {
            seq_t prev = cache[length - 1];

            /* compute number of divisors of prev (assume it fits in uint64_t) */
            uint64_t prev_u64 = (uint64_t)prev;
            uint64_t dcnt_u64 = divisor_count_u64(prev_u64);
            seq_t  dcnt      = (seq_t)dcnt_u64;

            seq_t cand = 1;

            for (;; ++cand) {
                /* check if cand has already been used */
                bool used = false;
                for (idx_t j = 0; j < length; ++j) {
                    if (cache[j] == cand) {
                        used = true;
                        break;
                    }
                }
                if (!used &&
                    gcd(prev, cand) > 1 &&
                    gcd(dcnt, cand) > 1)
                {
                    term = cand;
                    break;
                }
            }
        }

        /* grow the cache if necessary */
        if (length == capacity) {
            size_t newcap = (capacity == 0) ? 8 : capacity << 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();                 /* out of memory */
            cache    = tmp;
            capacity = newcap;
        }
        cache[length++] = term;
    }

    return cache[n - 1];
}
