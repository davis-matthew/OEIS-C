/*
 * A339846  Number of even-length factorizations of n into factors > 1.
 * https://oeis.org/A339846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into an even
 *            number of factors > 1 (empty factorization counted for n = 1)
 * Range:     u8 n <= 8639, u16 n <= 9665, u32 n <= 9665, u64 n <= 9665,
 *            u128 n <= 9665, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct { seq_t even; seq_t odd; } Pair;

static Pair factor_pair(idx_t n, idx_t minf)
{
    Pair res = {0, 0};
    if (n == 1) {
        res.even = 1;      /* empty product, length 0 (even) */
        return res;
    }
    for (idx_t d = minf; d <= n; ++d) {
        if (n % d != 0) continue;
        Pair sub = factor_pair(n / d, d);
        res.odd  += sub.even;   /* adding d flips parity */
        res.even += sub.odd;
    }
    return res;
}

seq_t a(idx_t n)
{
    return factor_pair(n, 2).even;
}
