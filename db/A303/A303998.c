/*
 * A303998  Number of ways to write 2*n+1 as p + 2^k + binomial(2*m,m), where
 *          p is a prime, and k and m are positive integers.
 * https://oeis.org/A303998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to represent 2*n+1 as p + 2^k +
 *            binomial(2*m, m), where p is prime, k>0 and m>0
 * Range:     u8 n <= 722, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9231 (2026-11-03)
 * Helpers:   binomial, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions are defined in the utils library. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
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

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for safety return 0 otherwise. */
    if (n < 1) return 0;

    /* Target value X = 2*n + 1.  Compute in seq_t to avoid overflow. */
    seq_t X = 2 * ((seq_t)n) + 1;

    seq_t count = 0;

    /* Loop over integer m >= 1 while binomial(2*m, m) <= X. */
    for (idx_t m = 1;; ++m) {
        seq_t C = binomial(2 * m, m);     /* binomial(2m, m) */
        if (C > X) break;              /* larger m cannot satisfy the equation */
        if (C == 0) break;             /* overflow sign; safe exit           */

        seq_t max_pow = X - C;          /* maximum possible value of 2^k   */

        /* Start with the smallest power of two allowed (2^1). */
        seq_t pow2k = 2;
        while (pow2k <= max_pow) {
            seq_t p = X - C - pow2k;    /* candidate prime part          */
            if (p >= 2 && is_prime_big(p))
                ++count;                /* one valid representation       */

            /* Advance to next power of two, avoiding overflow. */
            if (pow2k > max_pow / 2) break;
            pow2k <<= 1;
        }
    }

    return count;
}
