/*
 * A393460  a(n) is the least prime p such that n - p is the reverse of a
 *          prime, where leading 0's are not allowed, or -1 if there is no
 *          such p.
 * https://oeis.org/A393460
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign, base, look
 * Formula:   not stated
 * Range:     i8 n <= 286, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: least prime p such that n - p is the reverse of a prime. */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)-1;

    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime(p))
            continue;

        idx_t r = n - p;
        if (r == 0 || (r % 10) == 0)
            continue;

        idx_t rev_r = rev(r);
        if (is_prime(rev_r))
            return (seq_t)p;
    }

    return (seq_t)-1;
}
