/*
 * A336974  Sum of the largest two side lengths of all distinct integer-sided
 *          triangles with perimeter n.
 * https://oeis.org/A336974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{a=1}^{⌊n/3⌋} (n‑a) * #{b | a≤b≤⌊(n‑a)/2⌋,
 *            b>⌊(n‑2a)/2⌋}
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = Σ_{a=1}^{⌊n/3⌋} (n‑a) * (max(0, ⌊(n‑a)/2⌋ – max(a,⌊(n‑2a)/2⌋+1) + 1)) */
#include <stdint.h>
#include <stdlib.h>

/* External linkage */
seq_t a(idx_t n)
{
    if (n < 3) {
        /* No integer‑sided triangle with perimeter < 3 */
        return 0;
    }

    seq_t sum = 0;

    idx_t a_max = n / 3;                /* floor(n/3) */

    for (idx_t a = 1; a <= a_max; ++a) {

        /* b_min = max( a , floor((n - 2a)/2) + 1 ) */
        idx_t tmp = (n - 2*a) / 2;          /* floor((n-2a)/2) */
        idx_t b_min = tmp + 1;
        if (b_min < a) {
            b_min = a;
        }

        /* b_max = floor((n - a)/2) */
        idx_t b_max = (n - a) / 2;

        if (b_min <= b_max) {
            idx_t count = b_max - b_min + 1;
            seq_t diff = (seq_t)(n - a);
            sum += diff * (seq_t)count;
        }
    }

    return sum;
}
