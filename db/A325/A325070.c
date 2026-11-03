/*
 * A325070  Prime numbers congruent to 9 modulo 16 representable by x^2 +
 *          64*y^2.
 * https://oeis.org/A325070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325070: prime numbers p ≡ 9 (mod 16) that can be written as
   x^2 + 64*y^2.  The routine searches sequentially for the n‑th such
   prime; no caching is used.  The algorithm is applicable for all
   supported seq_t types, including arbitrary-precision integers. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Test whether a prime p satisfies p = x^2 + 64*y^2. */
static bool representable_by_64(seq_t p)
{
    seq_t max_y = floor_sqrt(p / 64);
    for (seq_t y = 0; y <= max_y; ++y) {
        seq_t term = 64 * y * y;
        seq_t rem  = p - term;
        seq_t sq   = floor_sqrt(rem);
        if (sq * sq == rem)
            return true;
    }
    return false;
}

/* ---------------------------------------------------------------
   Return the n‑th term of A325070 (1‑based index).
   --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* behavior below offset is irrelevant */

    idx_t   found   = 0;          /* how many valid primes we have found */
    seq_t   candidate = 2;        /* start with the smallest prime          */

    while (found < n) {
        /* Test candidate for primality */
        if (is_prime_big(candidate)) {
            /* Check congruence condition */
            if ((candidate % 16) == 9) {
                /* Check representation condition */
                if (representable_by_64(candidate)) {
                    found++;
                    if (found == n)
                        return candidate;
                }
            }
        }

        /* Advance to next candidate:
           after 2, only odd numbers are possible */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }

    /* Should never reach here */
    return 0;
}
