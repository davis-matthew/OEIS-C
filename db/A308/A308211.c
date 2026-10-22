/*
 * A308211  Number of integer-sided triangles with perimeter n and sides a, b
 *          and c such that a <= b <= c and the length of side a is coprime to
 *          c.
 * https://oeis.org/A308211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer-sided triangles with perimeter n * with sides
 *            a <= b <= c, a+b>c, and gcd(a,c)=1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t count = 0;

    idx_t a_max = n / 3;  /* a <= floor(n/3) */
    for (idx_t a = 1; a <= a_max; ++a)
    {
        /* Lower bound for b from a <= b and a+b > n/2: b > (n-2a)/2 */
        idx_t lower_b_excl = (n - 2 * a) / 2;
        idx_t lower_b = lower_b_excl + 1;
        if (lower_b < a) lower_b = a;

        /* Upper bound for b from b <= c: b <= (n-a)/2 */
        idx_t upper_b = (n - a) / 2;

        if (lower_b > upper_b) continue;

        /* Iterate over feasible b values */
        for (idx_t b = lower_b; b <= upper_b; ++b)
        {
            idx_t c = n - a - b;   /* c automatically >= b by construction */

            /* Check coprimality of a and c */
            if (gcd((seq_t)a, (seq_t)c) == 1)
                ++count;
        }
    }

    return count;
}
