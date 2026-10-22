/*
 * A303110  Number of partitions of n into two parts (p,q), with p < q, such
 *          that neither p+q nor |q-p| is semiprime.
 * https://oeis.org/A303110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{p=1..floor((n-1)/2)} (1 - [Omega(n-2p)=2]) * (1 - [Omega(n)=2])
 * Equivalent: if Omega(n)=2 then a(n)=0 else a(n)=#{p | 1≤p≤floor((n-1)/2) and Omega(n-2p)≠2}
 */

#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* This function assumes n ≥ 1 (as per OEIS offset). */
    seq_t omega_n = bigomega(n);
    if (omega_n == 2)
        return 0; /* n is semiprime → all partitions invalid */

    idx_t limit = (n - 1) / 2;          /* highest p with p < q */
    seq_t count = 0;

    for (idx_t p = 1; p <= limit; ++p)
    {
        idx_t t = n - 2 * p;             /* |q - p| = n - 2p */
        if (bigomega(t) != 2)
            ++count;
    }

    return count;
}
