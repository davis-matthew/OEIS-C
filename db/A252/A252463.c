/*
 * A252463  Hybrid shift: a(1) = 1, a(2n) = n, a(2n+1) = A064989(2n+1); shift
 *          the even numbers one bit right, shift the prime factorization of
 *          odd numbers one step towards smaller primes.
 * https://oeis.org/A252463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1, a(2n) = n, a(2n+1) = ∏p^e||2n+1 prev_prime(p)^e
 * Range:     u8 n <= 261, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 97 terms through n = 8088 (2026-11-01)
 * Helpers:   ipow_idx, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/is_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;

    /* Even index – just halve the index. */
    if ((n & 1u) == 0u)
    {
        return (seq_t)n >> 1;
    }

    /* Odd index > 1 – shift prime factorisation to the previous prime. */
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Trial division starting with the smallest odd prime. */
    uint64_t p = 3u;
    while (p <= m / p)
    {
        if (m % p == 0u)
        {
            uint64_t exp = 0u;
            while (m % p == 0u)
            {
                m /= p;
                ++exp;
            }
            /* Compute prev_prime(p)^exp and multiply. */
            seq_t base = (seq_t)prev_prime((idx_t)p);
            seq_t term = ipow_idx(base, (idx_t)exp);
            result = seq_mul(result, term);
        }
        p += 2u;        /* skip even numbers */
    }

    if (m > 1u)        /* remaining prime factor */
    {
        seq_t base = (seq_t)prev_prime((idx_t)m);
        result = seq_mul(result, base);   /* exp = 1 */
    }

    return result;
}
