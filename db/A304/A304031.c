/*
 * A304031  Number of ways to write 2*n as p + 2^k + 5^m with p prime and 2^k
 *          + 5^m a product of at most three distinct primes, where k and m
 *          are nonnegative integers.
 * https://oeis.org/A304031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1490, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304031.
 * a(n) = number of ways to write 2*n = p + 2^k + 5^m
 * where p is prime, k,m ≥ 0, and 2^k + 5^m is square‑free and has at most three distinct primes. */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"

static bool is_squarefree_at_most_three(uint64_t x)
{
    if (x < 2) return true;          /* 1 is treated as square‑free */

    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64(x, primes, exps, &cnt);

    if (cnt > 3) return false;       /* more than three distinct primes */

    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1) return false;   /* repeated prime → not square‑free */
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* offset 1, behaviour for n<1 irrelevant */

    /* Work in 64‑bit arithmetic; the tests use n up to ≈10^10 (fits). */
    uint64_t two = n * 2ULL;          /* 2*n */
    /* Limit for 2^k + 5^m to keep p ≥ 2 .  We use the largest 64‑bit value if
       two exceeds UINT64_MAX (unlikely for the tests). */
    uint64_t limit = (two >= 2ULL ? two - 2ULL : 0ULL);
    if (limit > UINT64_MAX) limit = UINT64_MAX;

    seq_t count = 0;

    for (uint64_t pow2 = 1ULL; pow2 <= limit; ) {
        uint64_t max_pow5 = limit - pow2;
        if (max_pow5 < 1ULL) break;            /* no valid 5^m can fit */

        for (uint64_t pow5 = 1ULL; pow5 <= max_pow5; ) {
            uint64_t s = pow2 + pow5;          /* candidate for 2^k + 5^m */
            if (s > limit) break;              /* safety, should not happen */

            if (is_squarefree_at_most_three(s)) {
                uint64_t p = two - s;
                if (p >= 2ULL) {
                    seq_t p_big = (seq_t)p;     /* works for both built‑in
                                                 * and big integer types */
                    if (is_prime_big(p_big))
                        ++count;
                }
            }

            /* next 5^m, guard against overflow */
            if (pow5 > UINT64_MAX / 5ULL) break;
            pow5 *= 5ULL;
        }

        /* next 2^k, guard against overflow */
        if (pow2 > UINT64_MAX / 2ULL) break;
        pow2 <<= 1U;
    }

    return count;
}
