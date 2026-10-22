/*
 * A393613  a(n) = least positive integer k such that prime(n) + 2^k + 4 is
 *          prime, or -1 if no such prime exists.
 * https://oeis.org/A393613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k>0 such that prime(n) + 2^k + 4 is prime * Return
 *            -1 if no such k exists (within a reasonable search limit)
 * Range:     i8 n <= 21, i16 n <= 60, i32 n <= 60, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   get_prime, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* below the OEIS offset */
        return (seq_t)-1;

    /* Upper bound on k for practical search – empirical evidence suggests
     * that all known instances have k <= 1000, and n <= 10000. */
    const idx_t K_LIMIT = 1000;

    seq_t p = (seq_t)get_prime(n);

    for (idx_t k = 1; k <= K_LIMIT; ++k) {
        seq_t cand = p + pow2(k) + 4;
        if (is_prime_big(cand))
            return (seq_t)k;
    }

    /* No prime found within the search limit; declare failure. */
    return (seq_t)-1;
}
