/*
 * A352683  a(n) = A352682(4, n).
 * https://oeis.org/A352683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} binomial(n-1, k-1) * a(n-k) initial values:
 *            a(0)=1, a(1)=4
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cache for memoisation; large enough for all n used by the tests (≤ 10000) */
    static seq_t cache[20001];
    static idx_t len = 0;                /* number of computed entries: cache[0 .. len-1] */

    /* initialise base cases once */
    if (len == 0) {
        cache[0] = 1;
        cache[1] = 4;
        len = 2;
    }

    /* return cached value if available */
    if (n < len)
        return cache[n];

    /* extend cache up to requested n */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0 || i == 1)          /* already initialised */
            continue;

        seq_t sum   = 0;
        seq_t binom = 1;               /* binomial(i-1, 0)    */
        idx_t  m    = i - 1;           /* numerator for binomial */

        for (idx_t j = 0; j < i; ++j) {
            sum += cache[j] * binom;
            if (j + 1 < i)             /* update to binomial(i-1, j+1) */
                binom = binom * (seq_t)(m - j) / (seq_t)(j + 1);
        }
        cache[i] = sum;
    }

    len = n + 1;
    return cache[n];
}
