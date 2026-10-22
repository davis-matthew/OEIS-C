/*
 * A377396  Expansion of e.g.f. (1 + log(1+x))^3.
 * https://oeis.org/A377396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..3} k! * binomial(3,k) * Stirling1(n,k) (signed
 *            Stirling numbers of the first kind)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization of already computed terms */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    static size_t current_n = 0;
    /* store current Stirling numbers S1(n,0..3) */
    static seq_t s[4] = {0};

    /* constants */
    const seq_t fact[4]   = {1, 1, 2, 6};
    const seq_t binom3[4] = {1, 3, 3, 1};

    /* initialise on first call */
    if (memo == NULL) {
        memo_cap = 1;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0] = (seq_t)1;          /* a(0) = 1 */
        s[0] = (seq_t)1;             /* S1(0,0) = 1 */
        s[1] = s[2] = s[3] = (seq_t)0;
        current_n = 0;
    }

    /* expand cache if necessary */
    if (n >= memo_cap) {
        size_t new_cap = memo_cap == 0 ? 1 : memo_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed, return 0 */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* compute terms up to requested n */
    for (idx_t m = current_n + 1; m <= n; ++m) {
        seq_t new_s[4];
        new_s[0] = (seq_t)0;
        for (idx_t k = 1; k <= 3; ++k) {
            new_s[k] = s[k - 1] - (seq_t)(m - 1) * s[k];
        }
        /* compute a(m) from new_s */
        seq_t sum = new_s[0];
        for (idx_t k = 1; k <= 3; ++k)
            sum += new_s[k] * fact[k] * binom3[k];
        memo[m] = sum;
        /* update s for next iteration */
        for (idx_t k = 0; k < 4; ++k) s[k] = new_s[k];
    }

    current_n = n;
    return memo[n];
}
