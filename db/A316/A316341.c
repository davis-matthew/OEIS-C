/*
 * A316341  Characteristic function of the factorials 1!, 2!, 3!, ...
 * https://oeis.org/A316341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n equals k! for some integer k ≥ 1, otherwise 0
 * Range:     u8 n <= 9695, u16 n <= 9695, u32 n <= 9695, u64 n <= 9695,
 *            u128 n <= 9695, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* n = 0 is not a factorial */
        return (seq_t)0;

    /* factorial of 1 is 1 */
    idx_t fact = 1;
    if (fact == n)       /* n = 1 */
        return (seq_t)1;

    /* iterate k = 2,3,... */
    for (idx_t k = 2;; ++k) {
        /* if multiplying by k would exceed n, n cannot be a factorial */
        if (fact > n / k)
            return (seq_t)0;

        fact *= k;       /* fact now equals k! */

        if (fact == n)
            return (seq_t)1;
    }
}
