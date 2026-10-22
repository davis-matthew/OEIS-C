/*
 * A309341  a(n) = Sum_{j=1..n*(n-1)} (n*j mod (n+j)).
 * https://oeis.org/A309341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1}^{n(n-1)} (n*j mod (n+j))
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 339, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1. */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Use 64‑bit unsigned integers for intermediate arithmetic.
       They are large enough for n up to 10 000 (n^2 < 1e8, n^2*n < 1e12). */
    uint64_t nn = (uint64_t)n;           /* convert index to plain integer */
    uint64_t n2 = nn * nn;               /* n^2 */
    uint64_t sum = 0;

    /* k runs over n+1 .. n^2.  In this range
       n*j mod (n+j) == (k - (n^2 % k)) % k
       and the remainder is zero when n^2 % k == 0. */
    for (uint64_t k = nn + 1; k <= n2; ++k) {
        uint64_t rem = n2 % k;
        if (rem)                 /* add k - rem only if remainder non‑zero */
            sum += k - rem;
    }

    return (seq_t)sum;
}
