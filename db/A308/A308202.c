/*
 * A308202  Number of integer-sided triangles with perimeter n such that the
 *          length of the smallest side is coprime to n.
 * https://oeis.org/A308202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer‑sided triangles with perimeter n such that the
 *            length of the smallest side is coprime to n
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 2) return 0;

    seq_t total = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t a_val = 1; a_val <= n / 3; ++a_val)
    {
        /* Skip when the smallest side is not coprime to the perimeter */
        if (gcd((seq_t)a_val, n_seq) != 1)
            continue;

        /* Upper bound for the middle side: floor((n‑a)/2) */
        idx_t U = (n - a_val) >> 1;

        /* Lower bound from the triangle inequality a + b > n/2:
           b > (n - 2a)/2  →  b_min = floor((n-2a)/2)+1   (if n > 2a) */
        idx_t L = a_val;
        if (n > (a_val << 1))
        {
            idx_t L2 = ((n - (a_val << 1)) >> 1) + 1;
            if (L2 > L)
                L = L2;
        }

        if (L <= U)
            total += (seq_t)(U - L + 1);
    }

    return total;
}
