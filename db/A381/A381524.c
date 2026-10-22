/*
 * A381524  Smallest exponent of x of maximal coefficient (ignoring signs) in
 *          Hermite polynomial of order n.
 * https://oeis.org/A381524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A381524: smallest exponent of x having the
 *  maximal (in absolute value) coefficient in the Hermite polynomial
 *  H_n(x).  The coefficients of H_n are
 *
 *        f(e) = n! · 2^e / (k! · e!) ,  e = n-2k ,  k = (n-e)/2 .
 *
 *  By comparing successive exponents the recurrence
 *
 *        f(e+2) / f(e) = 4·k / ((e+2)(e+1))
 *
 *  is used.  Starting from the minimal exponent having the same parity
 *  as n, we increase e by two while the ratio is > 1.  The first e
 *  where the ratio ≤ 1 is the exponent of the maximal coefficient;
 *  when the ratio equals 1 there are two equal maxima and the smaller
 *  exponent (the one we already found) is returned, as required by the
 *  OEIS definition.
 *
 *  All arithmetic uses 64‑bit unsigned integers.  We cast to
 *  unsigned __int128 for the few products that could overflow 64 bits,
 *  making the routine safe for any n that fits in an idx_t.
 */

#include <stdbool.h> /* for bool, if needed */

seq_t a(idx_t n)
{
    /* The exponent must have the same parity as n. */
    idx_t e = n & 1ULL;             /* 0 or 1 */

    for (;;)
    {
        /* k = (n - e) / 2  (>=0 because e <= n) */
        idx_t k = (n - e) / 2ULL;

        /* ratio comparison: 4*k  vs  (e+2)*(e+1) */
        unsigned __int128 left  = (unsigned __int128)4ULL * k;
        unsigned __int128 right = (unsigned __int128)(e + 2ULL) * (e + 1ULL);

        if (left > right)
        {
            e += 2ULL;            /* still increasing, try next even/odd step */
        }
        else
        {
            break;                /* max reached or tied, choose current e */
        }
    }

    return (seq_t)e;
}
