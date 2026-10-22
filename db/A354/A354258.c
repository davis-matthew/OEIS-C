/*
 * A354258  Earliest occurrence of n in A354257.
 * https://oeis.org/A354258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * rad(n) if n is odd or n ≡ 6 (mod 12); a(n) = (3/4) *
 *            n * rad(n) if n ≡ 2 or 10 (mod 12); a(n) = 2 * n * rad(n) if n ≡
 *            0 (mod 4)
 * Range:     u8 n <= 9, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided rad(n) implementation. */
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* Offset 1 – a(1) is defined. The value for n==0 is unused,
       but returning 0 keeps the function well‑defined. */
    if (n == 0)
        return 0;

    seq_t rad_n = rad(n);            /* product of distinct primes of n */
    seq_t nseq  = (seq_t)n;
    seq_t prod  = nseq * rad_n;      /* base value n * rad(n) */

    /* Integer mod of unsigned types is defined for all idx_t. */
    if ((n & 1UL) || (n % 12UL == 6UL))  /* odd or 6 mod 12 */
        return prod;

    if (n % 12UL == 2UL || n % 12UL == 10UL)   /* 2 or 10 mod 12 – multiply by 3/4 */
    {
        prod = prod * 3;      /* still fits in seq_t for all tests   */
        prod = prod / 4;      /* the division is exact for these n   */
        return prod;
    }

    /* Otherwise n is divisible by 4 */
    return prod * 2;
}
