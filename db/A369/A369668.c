/*
 * A369668  Numbers that have an even number of prime factors with
 *          multiplicity and whose arithmetic derivative is of the form 4k+3.
 * https://oeis.org/A369668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 4121, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A369668
   a(n) = numbers with an even number of prime factors (with multiplicity)
   whose arithmetic derivative ≡ 3 (mod 4).  Each term equals
   2 times the corresponding A359151 element.  The function builds the
   list via trial factorisation of odd candidates m, using
   n = 2*m.  For each candidate m we
     – compute ω(m); it must be odd (so 1+ω(m) is even).
     – compute the arithmetic derivative n' of n = 2*m via
        n' = Σ_i a_i · n / p_i   (including the prime 2 with a_2 = 1).
     – keep n if n' ≡ 3 (mod 4).  All such n are 2 mod 4.
   The list is grown on demand and may be memoised between calls.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t length = 0;      /* number of terms stored */
    static size_t capacity = 0;    /* allocated capacity */
    static uint64_t candidate = 1; /* candidate odd m, starting from 1 */

    /* Build the cache until it contains the n‑th term */
    while (length < (size_t)n)
    {
        uint64_t m = 0; /* current qualifying odd m */

        /* Find the next qualifying odd number m.  candidate
           remembers the last tested odd value – we start at
           1 so the first increment yields 3. */
        for (;;)
        {
            candidate += 2;                     /* keep odd */
            if (candidate < 3) { continue; }   /* safety for overflow */

            /* factorisation of m */
            uint64_t primes[64], exps[64];
            size_t cnt;
            factor_u64(candidate, primes, exps, &cnt);

            /* count prime factors with multiplicity */
            unsigned omega = 0;
            for (size_t i = 0; i < cnt; ++i) omega += exps[i];

            /* require odd ω(m) -> 1 + ω(m) even for n = 2*m */
            if ((omega & 1U) == 0) { continue; }

            /* n = 2*m (sequence value) */
            seq_t nval = (seq_t)2 * (seq_t)candidate;

            /* arithmetic derivative of n = 2*m */
            seq_t der = 0;

            /* contribution of prime 2 (a_2 = 1) */
            der += (seq_t)candidate;  /* n/2 = m */

            /* contributions of odd prime factors of m */
            for (size_t i = 0; i < cnt; ++i)
            {
                der += (seq_t)exps[i] * nval / (seq_t)primes[i];
            }

            /* keep the term if der ≡ 3 (mod 4) */
            if (der % (seq_t)4 == (seq_t)3)
            {
                m = candidate;
                break;
            }
        }

        /* store the found term */
        if (length == capacity)
        {
            size_t newcap = capacity ? capacity * 2 : 32;
            seq_t *tmp = (seq_t *)realloc(cache,
                                            newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            capacity = newcap;
        }
        cache[length++] = (seq_t)2 * (seq_t)m;   /* n = 2*m */
    }

    /* return the n-th term (1‑based indexing) */
    return cache[(size_t)n - 1];
}
