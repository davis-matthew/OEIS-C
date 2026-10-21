/*
 * A246590  Even numbers whose odd part is of the form 4m+3; Numbers missing
 *          from A241816.
 * https://oeis.org/A246590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*A091067(n) (nth even integer whose odd part ≡ 3 mod 4)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t num = 2; ; num += 2) {        /* iterate over even numbers only */
        seq_t odd = num;
        /* strip powers of two to obtain the odd part */
        while ((odd & 1) == 0)
            odd >>= 1;                      /* shift by integer 1 */
        /* check if odd part ≡ 3 (mod 4) */
        if ((odd & 3) == 3) {
            if (++found == n)
                return num;
        }
    }
}
