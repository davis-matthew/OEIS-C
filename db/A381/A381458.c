/*
 * A381458  Primes p such that p/prevprime(p) < 1 + (1/PrimePi(p)).
 * https://oeis.org/A381458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 3494, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A381458: primes p such that p/prevprime(p) < 1 + 1/PrimePi(p)
 * Computation:  p * π(p) < prev(p) * (π(p)+1)
 * We generate primes sequentially and test the inequality.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;

    /* static state for prime generation */
    static uint64_t prev_prime = 0;    /* previous prime (0 if none) */
    static idx_t pi_count = 0;         /* PrimePi(p) for current prime */
    static uint64_t candidate = 2;     /* next number to test for primality */

    /* generate terms until we have at least n of them */
    while (len < n)
    {
        /* find next prime starting from candidate */
        uint64_t cand = candidate;
        if (cand <= 2)
            cand = 2;
        else if (cand != 2 && cand % 2 == 0)
            cand++;                     /* make it odd */

        while (!is_prime_small((idx_t)cand))
            cand += 2;                  /* skip even numbers */

        /* cand is a prime */
        seq_t p_seq = (seq_t)cand;      /* cast to seq_t */
        pi_count++;                    /* update PrimePi(p) */

        /* test the inequality only when we have a previous prime */
        if (prev_prime > 0)
        {
            bool good;
            /* use larger built‑in type if seq_t is small to avoid overflow */
            if (sizeof(seq_t) <= 8) {
                unsigned long long lhs = (unsigned long long)p_seq * (unsigned long long)pi_count;
                unsigned long long rhs = (unsigned long long)prev_prime *
                                         ((unsigned long long)pi_count + 1ULL);
                good = lhs < rhs;
            } else {
                seq_t lhs = p_seq * ((seq_t)pi_count);
                seq_t rhs = prev_prime * (((seq_t)pi_count) + (seq_t)1);
                good = lhs < rhs;
            }
            if (good)
                append_candidate(&cache, &len, &cap, p_seq);
        }

        /* prepare for next iteration */
        prev_prime = cand;
        candidate = cand + 1;          /* next search starts at p+1 */
    }

    return cache[n - 1];
}
