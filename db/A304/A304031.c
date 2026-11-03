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
 * Validated: standard, pass, 106 terms through n = 9991 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304031.
 * a(n) = number of ways to write 2*n = p + 2^k + 5^m
 * where p is prime, k,m ≥ 0, and 2^k + 5^m is square‑free and has at most three distinct primes. */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
