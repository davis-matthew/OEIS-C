/*
 * A318656  The 2-adic valuation of ratio A318649(n)/A318512(n); a(n) =
 *          2*A007814(n) - A046645(n).
 * https://oeis.org/A318656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = 2*A007814(n) - A046645(n) Additive with a(2^e) =
 *            popcount(e), and a(p^e) = popcount(e) - 2*e for odd prime p
 * Range:     i8 n <= 9701, i16 n <= 9701, i32 n <= 9701, i64 n <= 9701,
 *            i128 n <= 9701, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes the 2-adic valuation of ratio A318649(n)/A318512(n). */
seq_t a(idx_t n)
{
    if (n == 0)   /* out-of-range, not used by OEIS */
        return 0;

    seq_t result = 0;
    uint64_t t = (uint64_t)n;

    /* Handle factor 2 separately: a(2^e) = popcount(e) */
    if ((t & 1ULL) == 0) {
        unsigned int e = 0;
        while ((t & 1ULL) == 0) {
            t >>= 1;
            ++e;
        }
        result += (seq_t)__builtin_popcountll(e);
    }

    /* Handle odd prime factors */
    for (unsigned long long p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            unsigned int e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            /* a(p^e) = popcount(e) - 2*e for odd p */
            result += (seq_t)__builtin_popcountll(e) - (seq_t)(2 * e);
        }
    }

    /* Remaining t > 1 is a prime factor with exponent 1 */
    if (t > 1) {
        /* e = 1, popcount(1) = 1, so a = 1 - 2*1 = -1 */
        result -= 1;
    }

    return result;
}
