/*
 * A354747  Start with 2*n-1; repeatedly triple and add 2 until reaching a
 *          prime. a(n) = number of steps until reaching a prime > 2*n-1, or 0
 *          if no prime is ever reached.
 * https://oeis.org/A354747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer m ≥ 1 such that 2*n*3^m – 1 is
 *            prime. Computation: start with x = 2*n – 1, then repeatedly
 *            compute x = 3*x + 2 until a prime is found or a preset step
 *            limit is reached. Return the found step count (as seq_t) or 0 if
 *            the limit is exceeded
 * Range:     u8 n <= 15, u16 n <= 20, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

#define MAX_STEPS 200000ULL   /* safety limit for very large n with no prime */

seq_t a(idx_t n)
{
    /* initial value: 2*n – 1 */
    seq_t x = (seq_t)2 * n - (seq_t)1;

    for (uint64_t m = 1; m <= MAX_STEPS; ++m) {
        /* apply the map x -> 3*x + 2 */
        x = x * (seq_t)3 + (seq_t)2;

        /* check primality */
        if (is_prime_big(x))
            return (seq_t)m;
    }

    /* reached the step limit – no prime found so far */
    return (seq_t)0;
}
