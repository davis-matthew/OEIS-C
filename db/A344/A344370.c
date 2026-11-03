/*
 * A344370  Dirichlet g.f.: Product_{k>=2} (1 + k^(1-s)).
 * https://oeis.org/A344370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * number of ways to express n as a product of distinct
 *            integers ≥ 2 (including n itself)
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_distinct_parts(idx_t n, idx_t min_factor)
{
    /* Count factorizations of n into strictly increasing factors > min_factor. */
    seq_t res = 0;
    if (n > min_factor)                /* the singleton factorization [n] */
        res = 1;

    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            /* use d1 as next factor */
            if (d1 > min_factor && d1 < n) {
                res += count_distinct_parts(n / d1, d1);
            }
            /* use d2 as next factor (only if it is different from d1) */
            if (d2 > d1 && d2 > min_factor && d2 < n) {
                res += count_distinct_parts(n / d2, d2);
            }
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)                      /* a(1) = 1 */
        return (seq_t)1;
    seq_t cnt = count_distinct_parts(n, 1);
    return (seq_t)n * cnt;
}
