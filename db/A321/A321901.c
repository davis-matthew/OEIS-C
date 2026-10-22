/*
 * A321901  Irregular table read by rows: T(n,k) = (2*k+1)^(-(2*k+1)) mod 2^n,
 *          0 <= k <= 2^(n-1) - 1.
 * https://oeis.org/A321901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r = floor(log2(n)) + 1, k = n - 2^(r-1), and
 *            T(r,k) = (2*k+1)^(-(2*k+1)) (mod 2^r)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the multiplicative inverse of (2*k+1)^(2*k+1)
   modulo 2^r by first finding the inverse of 2*k+1 (odd) and then
   raising it to the (2*k+1)‑th power.\n
   It works for all unsigned integer seq_t types and for arbitrary
   precision integer types. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine r = floor(log2(n)) + 1  (r >= 1) */
    idx_t r = 1;
    idx_t p = 2;           /* current 2^k, start with k = 1 */
    while (p <= n) {
        ++r;
        p <<= 1;
    }

    /* index k within row r (0‑based) */
    idx_t start = ((idx_t)1 << (r - 1));
    idx_t k = n - start;           /* 0 <= k <= 2^(r-1) - 1 */

    /* odd number whose (2*k+1)‑th power we need */
    seq_t m = ((seq_t)k << 1) + 1; /* m = 2*k + 1 */

    /* Compute inverse of m modulo 2^r by Hensel lifting */
    seq_t x = 1;          /* inverse modulo 2 (2^1) */
    for (idx_t i = 1; i < r; ++i) {          /* iterate for mod 2^(i+1) */
        seq_t mask = (seq_t)((((uint64_t)1 << (i + 1)) - 1)); /* 2^(i+1)-1 */
        seq_t m_mul = (m * x) & mask;            /* m * x (mod 2^(i+1)) */
        seq_t s     = (2 - m_mul) & mask;        /* 2 - m*x (mod 2^(i+1)) */
        x = (x * s) & mask;                     /* next inverse (mod 2^(i+1)) */
    }

    /* Epsilon: 2^r modulus and its mask */
    seq_t mask_final = (seq_t)((((uint64_t)1 << r) - 1));

    /* Compute T(r,k) = x^(2*k+1) (mod 2^r) */
    seq_t pow = 1;
    seq_t base = x & mask_final;
    seq_t exp = m;    /* exponent = 2*k+1 */

    while (exp) {
        if (exp & 1)
            pow = (pow * base) & mask_final;
        base = (base * base) & mask_final;
        exp >>= 1;
    }

    return pow;
}
