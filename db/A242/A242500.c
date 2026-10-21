/*
 * A242500  Number of compositions of n, where the difference between the
 *          number of odd parts and the number of even parts is 2.
 * https://oeis.org/A242500
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{e≥0, e≡n (mod 2), 3e≤n-2} C(2(e+1), e) * C( (n-3e-2)/2
 *            + 2(e+1)-1 , 2(e+1)-1 )
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 137,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts compositions of n whose difference (#odd parts – #even parts) = 2. */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t total = 0;

    /* upper bound for e: 3e ≤ n-2  →  e ≤ (n-2)/3  */
    idx_t maxE = (n >= 2) ? (n - 2) / 3 : 0;

    /* e must have the same parity as n (because n-e must be even) */
    idx_t startE = n & 1;           /* 0 if n even, 1 if n odd */
    for (idx_t e = startE; e <= maxE; e += 2)
    {
        /* k = number of parts in the composition = 2(e+1)  */
        idx_t k = 2 * (e + 1);

        /* S = (n - 3e - 2)/2  (must be integer, guaranteed by parity condition) */
        idx_t temp = n - 3 * e - 2;
        if (temp % 2) continue;      /* safety check – should never happen */
        idx_t S = temp / 2;

        /* Number of ways to choose which of the k positions are even parts */
        seq_t comb1 = binom(k, e);

        /* Number of non‑negative solutions to the remaining sum (S) over k parts */
        seq_t comb2 = binom(S + k - 1, k - 1);

        total += comb1 * comb2;
    }

    return total;
}
