/*
 * A308229  Sum of the perimeters of all triangles with perimeter n whose side
 *          lengths are mutually coprime.
 * https://oeis.org/A308229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A308074(n) (sum of the perimeters of all triangles
 *            with perimeter n whose side lengths are pairwise coprime)
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* no triple can form a triangle */

    /* counter of primitive triangles (a ≤ b ≤ c, a+b>c, a+b+c = n) */
    idx_t count = 0;

    /* a can be at most floor(n/3) */
    idx_t max_a = n / 3;
    idx_t half = n / 2;                      /* floor(n/2) */

    for (idx_t a = 1; a <= max_a; ++a) {
        /* b ranges from a up to floor((n - a)/2) to keep b ≤ c */
        idx_t max_b = (n - a) / 2;
        if (max_b < a) continue;             /* no b satisfying a ≤ b ≤ c */

        /* enforce triangle inequality a + b > n/2 */
        idx_t min_b = a;
        if (half >= a) {
            idx_t temp = half - a;
            if (temp + 1 > min_b) min_b = temp + 1;
        }

        if (min_b > max_b) continue;         /* no valid b for this a */

        for (idx_t b = min_b; b <= max_b; ++b) {
            idx_t c = n - a - b;             /* automatically c ≥ b */

            /* check pairwise coprimality */
            if (gcd((seq_t)a, (seq_t)b) != 1) continue;
            if (gcd((seq_t)a, (seq_t)c) != 1) continue;
            if (gcd((seq_t)b, (seq_t)c) != 1) continue;

            ++count;
        }
    }

    /* each triangle contributes its perimeter n to the sum */
    seq_t n_seq = (seq_t)n;
    seq_t result = n_seq * (seq_t)count;
    return result;
}
