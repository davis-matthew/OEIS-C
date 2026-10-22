/*
 * A368527  a(n) = Sum_{k=1..n} k^3 * n^k.
 * https://oeis.org/A368527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=1..n} k³ * nᵏ
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes each term iteratively using exact arithmetic in seq_t */

seq_t a(idx_t n)
{
    /* For n = 0 the sum is empty */
    if (n == 0ULL)
        return (seq_t)0;

    seq_t sum = 0;
    seq_t current = (seq_t)n;          /* current = n^k  (starting with k=1) */
    seq_t sn = (seq_t)n;               /* a copy of n for repeated use */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t k3 = kk * kk * kk;       /* k³ */
        sum += k3 * current;           /* add k³ * nᵏ */
        if (k < n)
            current *= sn;             /* advance to n^{k+1} */
    }

    return sum;
}
