/*
 * A271079  Residues (mod 32) of partial sums of Fibonacci numbers starting
 *          with F(2).
 * https://oeis.org/A271079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(n+4)-2) mod 32
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the pair (F(n), F(n+1)) modulo 32 using fast doubling.
   The results are written into *f and *f1. */
static void fib_pair_mod32(idx_t n, uint32_t *f, uint32_t *f1)
{
    if (n == 0) {
        *f = 0;
        *f1 = 1;
        return;
    }

    uint32_t a, b, c, d;
    fib_pair_mod32(n >> 1, &a, &b);

    /* c = F(2k)   = a * (2*b - a)   (mod 32) */
    uint32_t two_b = (b << 1) & 31;               /* 2*b mod 32 */
    uint32_t diff  = (two_b >= a) ? (two_b - a) : (two_b + 32 - a);
    c = (a * diff) & 31;

    /* d = F(2k+1) = a*a + b*b       (mod 32) */
    d = ((a * a) + (b * b)) & 31;

    if ((n & 1) == 0) {           /* even n */
        *f  = c;
        *f1 = d;
    } else {                      /* odd n */
        *f  = d;
        *f1 = (c + d) & 31;
    }
}

seq_t a(idx_t n)
{
    /* Reduce the index modulo the Pisano period 48 for modulus 32. */
    idx_t idx = (n % 48ULL + 4ULL) % 48ULL;

    uint32_t fn, dummy;
    fib_pair_mod32(idx, &fn, &dummy);

    /* Compute (F(n+4)-2) mod 32, i.e., (fn - 2) mod 32. */
    uint32_t res = (fn + 30) & 31;   /* fn - 2 ≡ fn + 30 (mod 32) */

    return (seq_t)res;
}
