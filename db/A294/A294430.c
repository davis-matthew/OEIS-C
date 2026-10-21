/*
 * A294430  Number of zero-energy ground states for SUSY SYK model of
 *          one-dimensional fermions with n sites.
 * https://oeis.org/A294430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1 a(2k) = 4 * 3^(k-1) (k ≥ 1) a(2k+1) = 2 * 3^k if k is
 *            odd a(2k+1) = 2 * 3^k + 2 if k ≡ 2 (mod 4) a(2k+1) = 2 * 3^k + 6
 *            if k ≡ 0 (mod 4) and k > 0 (This covers all n ≥ 0.)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial first terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Even n (n ≥ 2) */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;                 /* k = n/2, k ≥ 1 */
        seq_t p = seq_pow((seq_t)3, k - 1);
        return (seq_t)4 * p;              /* 4 * 3^(k-1) */
    }

    /* Odd n (n ≥ 3) */
    idx_t k = (n - 1) >> 1;                /* k = (n-1)/2 */
    seq_t p = seq_pow((seq_t)3, k);        /* 3^k */
    seq_t val = (seq_t)2 * p;              /* 2 * 3^k */

    if ((k & 1) == 0) {                   /* k even */
        if ((k & 3) == 0) {               /* k ≡ 0 (mod 4), k > 0 */
            val += (seq_t)6;
        } else {                          /* k ≡ 2 (mod 4) */
            val += (seq_t)2;
        }
    }
    /* For k odd, val already equals 2 * 3^k */

    return val;
}
