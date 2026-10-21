/*
 * A284921  Numbers with digits 2 and 7 only.
 * https://oeis.org/A284921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th smallest positive integer whose decimal
 *            representation uses only the digits 2 and 7, in increasing order
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is enumerated by first listing all length‑1 numbers (22),
   then all length‑2 numbers (22,27,72,77), then length‑3 numbers, and so on.
   For a fixed length L there are exactly 2^L such numbers, and their numeric
   order corresponds to the binary order of the string obtained by
   interpreting 2 as 0 and 7 as 1. */

#include <stdint.h>

/* Note: idx_t is an unsigned integer type supplied by the build.
   seq_t is an unsigned integer or arbitrary‑precision integer type
   supplied by the build. */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n < 1, return 0 as a harmless placeholder. */
        return 0;
    }

    /* Find the length L of the n-th number.  With 2^1 + 2^2 + … + 2^L
       = 2^(L+1) – 2 the cumulative count of numbers of length ≤ L.  We
       need the smallest L such that 2^(L+1) – 2 ≥ n.  Use 128‑bit
       arithmetic to avoid overflow for large indices. */
    unsigned __int128 pow2 = 2;     /* 2^1 */
    unsigned __int128 cumulative = 0;
    idx_t L = 0;

    while (1) {
        cumulative += pow2;
        L++;
        if (cumulative >= n) break;
        pow2 <<= 1;     /* next power of two */
    }

    /* Count of numbers with length less than L. */
    idx_t cumulative_before = (idx_t)(cumulative - pow2);   /* 2^L – 2 */

    /* Zero‑based offset within the block of numbers of length L. */
    idx_t offset0 = n - cumulative_before - 1;               /* 0 … 2^L-1 */

    /* Generate the decimal value by mapping bits of offset0 to 2 or 7. */
    seq_t val = 0;
    idx_t i;
    for (i = 0; i < L; ++i) {
        /* Process bits from most significant to least significant. */
        unsigned bit = (offset0 >> (L - 1 - i)) & 1;
        val = val * 10 + (bit ? 7 : 2);
    }

    return val;
}
