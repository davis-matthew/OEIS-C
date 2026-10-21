/*
 * A263928  Integers m such that sigma(m)^2 is divisible by m.
 * https://oeis.org/A263928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 34, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A263928 (OEIS): integers m such that σ(m)^2 is divisible by m. */
/* The function a(n) returns the nth term (1‑based index).  The implementation
   is a memoised brute‑force search: it tests successive integers m, computes
   σ(m) by factorisation, checks whether σ(m)^2 mod m == 0, and appends any
   qualifying m to a dynamically growing static array.  Subsequent calls to
   a() are answered in O(1) once the required terms have been generated. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* factor_u64, uint64_t utilities */
#include "../utils/pow_seq.h"      /* pow_seq(seq_t, int) */

#ifndef MAX_PRIMES_IN_FACTOR
#define MAX_PRIMES_IN_FACTOR 32   /* sufficient for uint64_t factorisation */
#endif

/* Compute σ(m) for a 64‑bit integer m.  The result is held in a seq_t. */
static inline seq_t sigma_of_uint64(uint64_t m)
{
    if (m == 1u)
        return (seq_t)1;

    uint64_t primes[MAX_PRIMES_IN_FACTOR];
    uint64_t exp[MAX_PRIMES_IN_FACTOR];
    size_t count = 0;

    factor_u64(m, primes, exp, &count);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t pe1 = pow_seq(p, (int)(exp[i] + 1));   /* p^(e+1) */
        seq_t term = (pe1 - (seq_t)1) / (p - (seq_t)1); /* (p^(e+1)-1)/(p-1) */
        result *= term;
    }
    return result;
}

/* Memoised generator of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;       /* array of found sequence values   */
    static size_t len     = 0;          /* number of values stored          */
    static size_t cap     = 0;          /* allocated capacity               */
    static uint64_t tested = 0;          /* largest integer that has been tested */

    while (len < (size_t)n) {
        ++tested;                       /* test next integer m          */

        seq_t s = sigma_of_uint64(tested);
        seq_t sq = s * s;
        if ((sq % (seq_t)tested) == (seq_t)0) {
            /* Grow array if necessary.  Double capacity to keep amortised O(1). */
            if (len + 1 > cap) {
                size_t newcap = cap ? 2 * cap : 8;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp)
                    abort();           /* allocation failure */
                terms = tmp;
                cap = newcap;
            }
            terms[len++] = (seq_t)tested;
        }
    }
    return terms[(size_t)n - 1];
}
