/*
 * A392084  a(n) = 3*a(n-1) + 2*3^n - 1, a(0) = 0.
 * https://oeis.org/A392084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n*(4*n - 1) + 1)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Compute 3^n in the seq_t domain */
    seq_t pw = 1;
    for (idx_t i = 0; i < n; ++i)
        pw *= (seq_t)3;

    /* Use a wide intermediate type to avoid overflow in the
       multiplication and division. __int128 is available on
       GCC/Clang and is sufficient for all seq_t sizes mentioned. */
    __int128 tmp = (__int128)pw * ((__int128)4 * (__int128)n - 1);
    tmp += 1;               /* add the +1 */
    tmp /= 2;               /* exact division */

    return (seq_t)tmp;
}
