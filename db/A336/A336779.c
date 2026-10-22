/*
 * A336779  a(n) is the largest power of n such that all numbers n^k <= a(n),
 *          k=1,..,A336778(n)-1 can be exactly represented as double precision
 *          64-bit floating point numbers according to the IEEE 754 standard.
 *          If a(n) is a power of 2, it is replaced by the corresponding
 *          negated exponent of 2.
 * https://oeis.org/A336779
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, fini, look
 * Formula:   a(n) = n^e where e is the largest integer such that * every
 *            power n^k (1 ≤ k ≤ e) is exactly representable as an IEEE-754 *
 *            64‑bit binary float. If n is a pure power of two, the result is
 *            * returned as the negative exponent of the power of two to avoid
 *            * printing gigantic numbers
 * Range:     i8 none, i16 n <= 2, i32 n <= 2, i64 n <= 5, i128 n <= 23,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Separate the power‑of‑two factor. */
    idx_t m = n;
    idx_t v = 0;                     /* exponent of 2 in n */
    while ((m & 1ULL) == 0) {
        m >>= 1;
        v++;
    }

    /* Pure powers of two are handled specially. */
    if (m == 1) {
        /* max k with v*k ≤ 1023 (largest finite exponent of binary64). */
        idx_t kmax = 1023 / v;
        /* Return the corresponding negative exponent of 2. */
        return -(seq_t)(v * kmax);
    }

    /* For n with odd part > 1, the representability condition reduces to
     *  o^k < 2^53
     * where o is the odd part of n.  Find the largest k satisfying this. */
    const unsigned long long LIMIT = 9007199254740992ULL; /* 2^53 */

    if (m > LIMIT) {
        /* odd part already exceeds 2^53 – only k = 0 works. */
        return (seq_t)1;
    }

    unsigned __int128 cur = 1;
    idx_t kmax = 0;
    while (1) {
        unsigned __int128 nxt = cur * (unsigned __int128)m;
        if (nxt >= (unsigned __int128)LIMIT)
            break;
        cur = nxt;
        kmax++;
    }

    /* Return n^kmax (exact integer, using arbitrary-precision routine). */
    return seq_pow((seq_t)n, kmax);
}
