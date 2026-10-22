/*
 * A332082  a(n) = Sum_{1 <= m <= n} Sum_{1 <= k <= n+1-m} m*R(k,n+1), where
 *          R(k,b) = (b^k - 1)/(b - 1) is the base-b repunit of length k.
 * https://oeis.org/A332082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{m=1..n} Σ_{k=1..n+1-m} m * R(k,n+1), * where R(k,b) =
 *            (b^k - 1)/(b - 1) is the base‑b repunit of length k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Base = n + 1 (so digits run from 0..n). */
    seq_t base = (seq_t)n + 1;

    seq_t repunit = 1;   /* R(1, base). */
    seq_t sum_rep = 0;   /* S(t) = Σ_{k=1..t} R(k,base). */
    seq_t result  = 0;   /* a(n). */

    /* Iterate over t = 1 .. n; weight = n+1-t. */
    for (idx_t t = 1; t <= n; ++t) {
        if (t > 1) {
            /* R(t,base) = R(t-1,base) * base + 1. */
            repunit = repunit * base + (seq_t)1;
        }
        sum_rep += repunit;                     /* S(t). */
        seq_t weight = (seq_t)(n + 1 - t);       /* n + 1 - t. */
        result += weight * sum_rep;             /* accumulate a(n). */
    }
    return result;
}
