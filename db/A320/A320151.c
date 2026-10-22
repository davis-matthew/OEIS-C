/*
 * A320151  Number of integer solutions to a^2 + 2*b^2 + 2*c^2 + 5*d^2 = n.
 * https://oeis.org/A320151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS: A320151
   a(n) = number of integer solutions to a^2 + 2*b^2 + 2*c^2 + 5*d^2 = n
   (solutions for a, b, c, d ∈ ℤ, counted with sign).
   The function computes this directly by iterating over the possible
   non‑negative values of |d|, |b|, |c| and checking whether the
   remaining term can be expressed as a perfect square in a.
   Each variable contributes a multiplicity of 1 if zero, otherwise 2.
*/
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 0; ; d++)
    {
        seq_t d_term = (seq_t)d * d * 5;
        if (d_term > n) break;
        seq_t d_mult = (d == 0 ? 1 : 2);
        seq_t rem_d = n - d_term;

        for (idx_t b = 0; ; b++)
        {
            seq_t term_b = (seq_t)b * b * 2;
            if (term_b > rem_d) break;
            seq_t b_mult = (b == 0 ? 1 : 2);
            seq_t rem_b = rem_d - term_b;

            for (idx_t c = 0; ; c++)
            {
                seq_t term_c = (seq_t)c * c * 2;
                if (term_c > rem_b) break;
                seq_t c_mult = (c == 0 ? 1 : 2);
                seq_t t = rem_b - term_c;           // t >= 0 by loop condition

                seq_t sq = floor_sqrt(t);
                if (sq * sq == t)
                {
                    seq_t a_mult = (t == 0 ? 1 : 2);
                    count += d_mult * b_mult * c_mult * a_mult;
                }
            }
        }
    }
    return count;
}
