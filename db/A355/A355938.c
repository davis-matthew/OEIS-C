/*
 * A355938  a(n) = 1 if sigma(n^2) is a noncomposite, otherwise 0.
 * https://oeis.org/A355938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n^2) is a noncomposite (1 or prime), otherwise
 *            0
 * Range:     u8 n <= 16, u16 n <= 1708, u32 n <= 1708, u64 n <= 9013,
 *            u128 n <= 9013, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1, undefined behavior below */
        return (seq_t)0;

    /* compute sigma(n^2) */
    /* n is uint64_t, safe for n up to 10^4 requested in the tests */
    idx_t sq = n * n;
    seq_t s = sigma_of(sq);

    /* noncomposite means 1 or a prime */
    if (s == (seq_t)1)
        return (seq_t)1;
    if (is_prime_big(s))
        return (seq_t)1;
    return (seq_t)0;
}
