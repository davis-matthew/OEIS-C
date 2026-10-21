/*
 * A264527  Largest number m such that (2*n-m, 2*n+m) is a prime pair.
 * https://oeis.org/A264527
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest m such that (2*n-m, 2*n+m) are both prime. The
 *            routine searches downward from the maximal odd m using the
 *            supplied is_prime(idx_t) primality test
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* value undefined for n < 2 */

    /* maximum possible odd m is 2*n-2 (since m < 2*n for the left prime to be > 0) */
    idx_t max_m = 2 * n - 2;
    if (max_m < 1)          /* safety for very small n */
        max_m = 1;
    if ((max_m & 1) == 0)     /* make it odd */
        --max_m;

    for (idx_t m = max_m; ; m -= 2) {
        /* compute the two members of the pair */
        idx_t left  = 2 * n - m;
        idx_t right = 2 * n + m;

        if (is_prime(left) && is_prime(right))
            return (seq_t)m;     /* found the largest m */

        if (m <= 1)            /* no smaller odd m to try */
            break;
    }

    return (seq_t)0;            /* should never be reached for n >= 2 */
}
