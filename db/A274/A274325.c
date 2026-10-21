/*
 * A274325  Number of partitions of n^5 into at most two parts.
 * https://oeis.org/A274325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + (n%2?1:2))/2 (From a(n) = (3 + (-1)^n + 2*n^5)/4 )
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If seq_t is 64‑bit or smaller we use 128‑bit arithmetic for
       intermediate calculations to avoid overflow. For larger seq_t
       types (128‑bit or arbitrary precision) we compute directly with
       seq_t operators. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit intermediate */
        unsigned __int128 n128 = n;
        unsigned __int128 n2   = n128 * n128;        /* n^2  */
        unsigned __int128 n4   = n2 * n2;            /* n^4  */
        unsigned __int128 n5   = n4 * n128;          /* n^5  */
        unsigned __int128 add  = (n & 1ULL) ? 1 : 2; /* +1 if odd, +2 if even */
        unsigned __int128 res128 = (n5 + add) / 2;   /* (n^5 + add)/2 */
        return (seq_t)res128;
    } else {
        /* seq_t is 128‑bit or arbitrary precision */
        seq_t n5 = (seq_t)n;
        n5 = n5 * n;   /* n^2 */
        n5 = n5 * n;   /* n^3 */
        n5 = n5 * n;   /* n^4 */
        n5 = n5 * n;   /* n^5 */
        seq_t add = (n & 1ULL) ? 1 : 2;
        return (n5 + add) / 2;
    }
}
