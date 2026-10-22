/*
 * A302906  a(0) = 0; for n > 0, a(n) = a(n-1) + 5*n + 4.
 * https://oeis.org/A302906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*n + 4, with a(0) = 0
 * Range:     u8 n <= 8, u16 n <= 160, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   which simplifies to a(n) = n*(5*n + 13)/2 for reference   */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;          /* a(0) = 0 */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* result += 5*i + 4  (all in seq_t arithmetic) */
        result += (seq_t)5 * (seq_t)i + (seq_t)4;
    }
    return result;
}
