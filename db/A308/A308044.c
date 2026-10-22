/*
 * A308044  a(n) = 2*prevprime(2*n-1) - 2*n, where prevprime(n) is the largest
 *          prime < n.
 * https://oeis.org/A308044
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prevprime(2*n-1) - 2*n
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; values for n < 2 are undefined.
       Returning 0 for n < 2 is harmless for any caller. */
    if (n < 2)
        return 0;

    /* Compute 2*n-1 and find the largest prime strictly less than that number. */
    idx_t two_n_minus_one = (n << 1) - 1;
    idx_t p = prev_prime(two_n_minus_one);

    /* Apply the formula: 2*p - 2*n.  The computation is performed in idx_t
       (uint64_t) to avoid overflow of the intermediate values.  The final
       result is cast to seq_t.  If the result fits in seq_t it is exact. */
    idx_t val = (p << 1) - (n << 1);
    return (seq_t)val;
}
