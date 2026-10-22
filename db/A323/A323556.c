/*
 * A323556  Irregular table read by rows: T(n,k) = (2*k+1)^(1/3) mod 2^n, 0 <=
 *          k <= 2^(n-1) - 1.
 * https://oeis.org/A323556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k), where * r = ceil(log2(n+1)) * k = n - 2^(r-1) *
 *            T(r,k) is the unique odd integer x modulo 2^r such that x^3 ≡
 *            2*k+1 (mod 2^r). * The cube root modulo 2^r is found by Hensel
 *            lifting from the modulus 2
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 for the sequence.  If n==0 we return 0 as a safe default. */
    if (n == 0) return (seq_t)0;

    /* Find r such that 2^(r-1) <= n < 2^r. */
    unsigned r = 0;
    idx_t pow2 = 1;                 /* pow2 = 2^r */
    while (pow2 <= n) {
        pow2 <<= 1;
        ++r;
    }
    idx_t row_start = pow2 >> 1;      /* 2^(r-1) */
    idx_t k = n - row_start;          /* 0‑based index within the row */
    seq_t a_val = (seq_t)(2ULL * k + 1ULL);

    /* Hensel lifting of the cubic root modulo 2^r. */
    seq_t x = 1;                      /* root modulo 2 (odd root of 1) */
    for (unsigned m = 1; m < r; ++m) {
        seq_t diff = a_val - (x * x * x);   /* a_val - x^3   (unsigned wrap is fine) */
        seq_t c    = diff >> m;             /* (a_val - x^3) / 2^m  (mod 2) */
        seq_t t    = c & 1;                 /* t = c mod 2            */
        x += (t << m);                     /* lift to modulus 2^(m+1) */
    }

    return x;     /* this is T(r,k) */
}
