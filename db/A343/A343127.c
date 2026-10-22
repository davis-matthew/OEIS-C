/*
 * A343127  Number of solutions to n = x + y + z in positive integers x,y,z
 *          such that (x+y) does not divide any of x*y, y*z, or x*z.
 * https://oeis.org/A343127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..⌊n/3⌋} Σ_{i=j..⌊(n-j)/2⌋} [(i*j) mod (i+j) ≠ 0] *
 *            [(i*(n-i-j)) mod (i+n-j-i) ≠ 0] * [(j*(n-i-j)) mod (j+n-i-j) ≠
 *            0] (indicator products)
 * Range:     u8 n <= 58, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Number of solutions to n = x + y + z in positive integers
   with (x+y) not dividing xy, yz, or xz. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t count = 0;

    for (idx_t j = 1; j <= n / 3; ++j) {
        idx_t max_i = (n - j) / 2;
        for (idx_t i = j; i <= max_i; ++i) {
            idx_t k = n - i - j;                    /* third variable */
            /* Since i ≤ max_i, we have i ≤ k automatically. */

            if ((i * j) % (i + j) != 0 &&
                (i * k) % (i + k) != 0 &&
                (j * k) % (j + k) != 0) {
                ++count;
            }
        }
    }

    return count;
}
