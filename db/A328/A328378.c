/*
 * A328378  Number of permutations of length n that possess the maximal sum of
 *          distances between contiguous elements.
 * https://oeis.org/A328378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2*n)=2*((n-1)! )^2 for n>0, *
 *            a(2*n+1)=4*n!*(n-1)! for n>0
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 27, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* For the maximal value of n = 10000, the largest factorial needed is
 * (n/2)!  so we preallocate storage for (5000)!.
 */
static seq_t fac[5001];
static size_t fac_len = 0;

/* Ensure that fac[0..up] are computed, with fac[i] = i! */
static void ensure_factorial(size_t up)
{
    if (fac_len == 0) {
        fac[0] = (seq_t)1;
        fac_len = 1;
    }
    while (fac_len <= up) {
        fac[fac_len] = fac[fac_len - 1] * (seq_t)fac_len;
        fac_len++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    if ((n & 1) == 0) {                /* n = 2 * k   */
        idx_t k = n / 2;
        ensure_factorial((size_t)(k) - 1);   /* need (k-1)! */
        seq_t f = fac[k - 1];               /* (k-1)! */
        return f * f * (seq_t)2;
    } else {                           /* n = 2 * k + 1  */
        idx_t k = (n - 1) / 2;                /* k >= 1  */
        ensure_factorial((size_t)k);          /* need k! and (k-1)! */
        seq_t f1 = fac[k];                   /* k! */
        seq_t f2 = fac[k - 1];               /* (k-1)! */
        return f1 * f2 * (seq_t)4;
    }
}
