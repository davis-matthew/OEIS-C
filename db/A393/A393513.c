/*
 * A393513  Number of partitions of n having three times as many odd parts as
 *          even.
 * https://oeis.org/A393513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A393513
    Number of partitions of n having three times as many odd parts as even.

    The generating function is
        G(x) = Σ_{k≥0} x^(5k) /
               ( Π_{j=1}^k (1-x^(2j))^2 · Π_{j=k+1}^{3k} (1-x^(2j)) ).

    We build the series coefficient of G(x) iteratively.
    For each k we multiply the current polynomial by the four
    factors 1/(1-x^m) with m = 2k, 2(3k-2), 2(3k-1), 2·3k.
    The coefficient of x^(n-5k) in the resulting polynomial
    gives the contribution of the term k to a(n).
    Summation over all k (0 ≤ 5k ≤ n) yields a(n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;                  /* array indices fit in size_t for given n */
    seq_t *coeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!coeff) return (seq_t)0;           /* out of memory – return 0 */

    for (size_t i = 0; i <= N; ++i) coeff[i] = 0;
    coeff[0] = 1;                          /* H_0(x) = 1 */

    seq_t ans = 0;
    size_t kmax = N / 5;                   /* largest k with 5k <= n */

    /* k = 0 */
    if (N >= 0) ans += coeff[N];

    for (size_t k = 1; k <= kmax; ++k) {
        size_t m1 = 2 * k;
        size_t m2 = 2 * (3 * k - 2);
        size_t m3 = 2 * (3 * k - 1);
        size_t m4 = 2 * 3 * k;            /* 6k */

        /* unbounded knapsack for added part sizes */
        if (m1 <= N) {
            for (size_t s = m1; s <= N; ++s)
                coeff[s] += coeff[s - m1];
        }
        if (m2 <= N) {
            for (size_t s = m2; s <= N; ++s)
                coeff[s] += coeff[s - m2];
        }
        if (m3 <= N) {
            for (size_t s = m3; s <= N; ++s)
                coeff[s] += coeff[s - m3];
        }
        if (m4 <= N) {
            for (size_t s = m4; s <= N; ++s)
                coeff[s] += coeff[s - m4];
        }

        size_t idx = N - 5 * k;
        if (idx <= N) ans += coeff[idx];
    }

    free(coeff);
    return ans;
}
