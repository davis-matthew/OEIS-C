/*
 * A272285  Primes of the form 43*n^2 - 537*n + 2971 in order of increasing
 *          nonnegative values of n.
 * https://oeis.org/A272285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime of the form 43*n² - 537*n + 2971 with n = n-1 (n≥1)
 * Range:     u8 none, u16 n <= 43, u32 n <= 3808, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_mul.h"        /* seq_mul(seq_t a, seq_t b) */
#include "../utils/mul_small.h"     /* mul_small(seq_t x, unsigned int m) */
#include "../utils/isPrimeGeneric.h"/* isPrimeGeneric(seq_t n) */

/* Returns the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) {            /* undefined: sequence starts at offset 1 */
        return (seq_t)0;
    }

    /* Persistent cache of found primes */
    static seq_t *primes = NULL;       /* array of primes found so far  */
    static size_t count   = 0;         /* number of primes stored        */
    static size_t cap     = 0;         /* current capacity of array      */
    static idx_t  next_n  = 0;         /* next n (non‑negative) to test  */

    /* Initialise the cache on first call */
    if (primes == NULL) {
        cap = 128;
        primes = (seq_t *)malloc(sizeof(seq_t) * cap);
    }

    /* Generate primes until we have at least n of them */
    while (count < n) {
        seq_t i = (seq_t)next_n;               /* i = current n (0‑based)           */
        seq_t i_sq = seq_mul(i, i);            /* i²                                 */
        seq_t term = mul_small(i_sq, 43);      /* 43*i²                              */
        term = term - mul_small(i, 537);       /* -537*i                            */
        term = term + (seq_t)2971;             /* +2971                             */

        if (isPrimeGeneric(term)) {
            if (count == cap) {                /* grow array if needed             */
                cap <<= 1;
                primes = (seq_t *)realloc(primes,
                                            sizeof(seq_t) * cap);
            }
            primes[count++] = term;            /* store the found prime            */
        }
        ++next_n;                              /* move to next index                */
    }

    /* Return the requested prime */
    return primes[n - 1];
}
