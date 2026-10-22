/*
 * A337588  Number of distinct positive integer pairs, (s,t), such that s < t
 *          < n where neither s nor t divides n, but s | t.
 * https://oeis.org/A337588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{(s,t) : 1 ≤ s < t < n, n mod s ≠ 0, n mod t ≠ 0, and s
 *            divides t }|
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache to avoid reallocating on each call */
static bool *divides = NULL;
static idx_t divides_cap = 0;

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* ensure the divisor table is large enough */
    if (divides_cap < n) {
        bool *p = (bool*)realloc(divides, n * sizeof(bool));
        if (!p) return 0;            /* allocation failure – return 0 */
        divides = p;
        divides_cap = n;
    }

    /* mark which numbers < n divide n */
    for (idx_t i = 1; i < n; ++i)
        divides[i] = (n % i == 0);

    seq_t count = 0;

    /* count qualifying pairs (s,t) */
    for (idx_t s = 1; s < n; ++s) {
        if (divides[s]) continue;    /* skip divisors of n */
        for (idx_t t = s + s; t < n; t += s) {
            if (!divides[t]) ++count;   /* t is a multiple of s and not a divisor of n */
        }
    }

    return count;
}
