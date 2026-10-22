/*
 * A319816  Number of partitions of n into exactly six positive triangular
 *          numbers.
 * https://oeis.org/A319816
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly six positive
 *            triangular numbers
 * Range:     u8 n <= 257, u16 n <= 4587, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Quick exit for n < 6 (offset of the sequence) */
    if (n < 6)
        return 0;

    /* Build list of triangular numbers <= n */
    idx_t cap = 64;
    idx_t size = 0;
    idx_t *tri = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!tri)
        return 0; /* allocation failure, return 0 as a safe fallback */

    for (idx_t j = 1;; j++) {
        seq_t t_big = (seq_t)j * (seq_t)(j + 1) / 2;
        if (t_big > n)
            break;
        idx_t t = (idx_t)t_big;
        if (size == cap) {
            cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(tri, cap * sizeof(idx_t));
            if (!tmp) {
                free(tri);
                return 0;
            }
            tri = tmp;
        }
        tri[size++] = t;
    }

    /* Allocate DP table: (n+1) sums * (6+1) part counts */
    seq_t *dp = (seq_t *)calloc((n + 1) * 7UL, sizeof(seq_t));
    if (!dp) {
        free(tri);
        return 0;
    }
    dp[0] = 1; /* one way to sum 0 with 0 parts */

    /* Standard partition DP: iterate over parts in increasing order */
    for (idx_t ii = 0; ii < size; ++ii) {
        idx_t t = tri[ii];
        for (idx_t s = t; s <= n; ++s) {
            idx_t base = s * 7;
            idx_t src  = (s - t) * 7;
            for (idx_t k = 1; k <= 6; ++k) {
                dp[base + k] += dp[src + (k - 1)];
            }
        }
    }

    seq_t result = dp[n * 7 + 6];

    free(dp);
    free(tri);
    return result;
}
