/*
 * A349089  a(n) = n! * Sum_{k=0..floor((n-1)/4)} 1 / (4*k+1)!.
 * https://oeis.org/A349089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k = 0 .. floor((n-1)/4)} 1 / (4*k+1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the OEIS sequence A349089 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute n! iteratively */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact = fact * (seq_t)i;

    seq_t term = fact;          /* term for k = 0  : n! / 1! = n!   */
    seq_t sum  = term;          /* initialise sum with the first term */

    /* Number of terms: floor((n-1)/4) */
    idx_t maxk = (n - 1) / 4;
    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t base = 4 * (k - 1);   /* base = 4*(k-1)   */

        /* denom = (4*(k-1)+2)*(4*(k-1)+3)*(4*(k-1)+4)*(4*(k-1)+5) */
        seq_t denom = (seq_t)(base + 2) *
                      (seq_t)(base + 3) *
                      (seq_t)(base + 4) *
                      (seq_t)(base + 5);

        term = term / denom;    /* exact integer division */
        sum  = sum + term;      /* accumulate */
    }

    return sum;
}
