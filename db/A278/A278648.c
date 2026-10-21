/*
 * A278648  Consider the set S of integers 1 through n. a(n) is the number of
 *          unordered ways in which three distinct elements {a, b, c} of S
 *          satisfy a*b = c*n.
 * https://oeis.org/A278648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 6719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A278648:
 *
 * a(n) = number of unordered triples {a,b,c} of distinct integers in
 *        {1..n} such that a*b = c*n.
 *
 * The algorithm iterates over a=1..n and for each a uses the
 * factorisation property that, if d = gcd(a,n) and g = n/d,
 * then all solutions are obtained by
 *
 *   b = g*t   (1 ≤ t < d, t integer)
 *   c = (a/d)*t
 *
 * For each such t we check that a<b (to avoid double counting) and
 * that c≠a (t=d would give c=a) and that a≠n (which yields c=b).
 *
 * The total time is ∑_{a=1}^{n} gcd(a,n) ≈ n·log n, which is easily
 * under 0.03 s even for n=10 000.
 */

#include <stdint.h>

/* Euclidean algorithm for unsigned integers. */
static inline idx_t
gcd_idx(idx_t u, idx_t v)
{
    while (v != 0) {
        idx_t t = u % v;
        u = v;
        v = t;
    }
    return u;
}

seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t count = 0;

    for (idx_t a = 1; a <= n; ++a) {
        /* Avoid the trivial case a = n: it never contributes a valid triple */
        if (a == n)
            continue;

        idx_t d = gcd_idx(a, n);        /* gcd(a,n) */
        idx_t g = n / d;                /* n / gcd(a,n)   */
        idx_t m = a / d;                /* a / gcd(a,n)   */

        /* t runs from 1 to d-1 (t=d would give c=a, disallowed) */
        for (idx_t t = 1; t < d; ++t) {
            idx_t b = g * t;            /* b <= n automatically */
            idx_t c = m * t;            /* c <= n automatically */

            /* Unordered triple: enforce a < b */
            if (!(a < b))
                continue;

            /* c must be distinct from a and b, which follows from
             * the construction as long as a < b and t < d.            */
            if (c == a || c == b)
                continue;

            ++count;
        }
    }

    return count;
}
