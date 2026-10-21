/*
 * A284760  a(n) = Sum_{i=1..n-1}(i^(n-2)) mod n^4.
 * https://oeis.org/A284760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n-1}(i^(n-2)) mod n^4
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 270, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* compute modulus n^4 */
    seq_t ns = (seq_t)n;
    seq_t mod = ns * ns;   /* n^2 */
    mod *= mod;            /* n^4 */

    seq_t sum = 0;
    idx_t exp = n - 2;

    /* iterate i = 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t term = mod_pow((seq_t)i, exp, mod);
        sum += term;
        if (sum >= mod)
            sum -= mod;
    }

    return sum;
}
