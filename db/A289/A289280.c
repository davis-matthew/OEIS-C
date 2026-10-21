/*
 * A289280  a(n) is the least integer k > n such that any prime factor of k is
 *          also a prime factor of n.
 * https://oeis.org/A289280
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least integer k > n such that every prime factor of
 *            k divides n (i.e., the least Pₙ‑smooth number > n)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the prime factorization helper. */
#include "../utils/factor_u64.h"

#include <stddef.h>
#include <stdbool.h>

/* Compute a(n) for n ≥ 2. */
seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; just return 0. */
    if (n < 2)
        return 0;

    /* Factor n into distinct primes and their exponents. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, pr, ex, &pcnt);

    /* If n is a prime power, a(n) = p^(k+1).  Compute it directly. */
    if (pcnt == 1)
    {
        seq_t p = (seq_t)pr[0];
        seq_t result = 1;
        for (size_t i = 0; i < ex[0] + 1; ++i)
            result *= p;
        return result;
    }

    /* For other n, search for the smallest smooth number > n. */
    seq_t cand = (seq_t)n + 1;
    for (;; ++cand)
    {
        seq_t temp = cand;
        /* Divide out all prime factors of n. */
        for (size_t i = 0; i < pcnt; ++i)
        {
            seq_t p = (seq_t)pr[i];
            while (temp % p == 0)
                temp /= p;
        }
        /* If remaining part is 1, all prime factors belong to n's set. */
        if (temp == 1)
            return cand;
        /* else, continue searching. */
    }
}
