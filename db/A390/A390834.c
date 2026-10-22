/*
 * A390834  The maximum number of proper divisors of a semiperfect number k
 *          that sum to k.
 * https://oeis.org/A390834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the maximum number of proper divisors of the n-th
 *            semiperfect number that sum to that number
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This routine slowly generates semiperfect numbers in ascending order.  For each candidate
 *  k it computes all proper divisors, their sum S, and the difference D = S − k.
 *  If S < k the number is deficient and is skipped.
 *  If S == k the number is perfect; the answer is the number of proper divisors.
 *  If S > k we solve a small 0‑1 knapsack problem to find the minimal number of divisors
 *  whose sum equals D.  The result is (number of divisors – minimal set size).
 *  The answers are memoised in a dynamic array so that calls can arrive in any order.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static idx_t cur_k = 2;                        /* next candidate to examine          */
    static seq_t *values = NULL;                   /* memoised answers                   */
    static size_t values_cap = 0;
    static idx_t computed_n = 0;

    static int *dpdiff = NULL;                     /* DP array for minimal removals      */
    static size_t dpdiff_cap = 0;
    const int INF = 0x3f3f3f3f;                    /* large value used as ∞              */

    /* -------------------------------------------------------------------- */
    /* generate all required terms up to n                                 */
    while (computed_n < n) {
        bool found = false;
        while (!found) {
            /* ---------------------------------------------------------------- */
            /* find all proper divisors of cur_k and their sum                  */
            seq_t sum = 0;
            int divcnt = 0;
            seq_t divs[300];                       /* sufficiently large for ≤ 1e6       */

            /* 1 is always a proper divisor (for k > 1)                         */
            if (cur_k > 1) {
                divs[divcnt++] = 1;
                sum += 1;
            }

            /* enumerate remaining divisors                                    */
            for (idx_t d = 2; d <= cur_k / d; ++d) {
                if (cur_k % d == 0) {
                    divs[divcnt++] = d;
                    sum += d;
                    idx_t other = cur_k / d;
                    if (other != d) {
                        divs[divcnt++] = other;
                        sum += other;
                    }
                }
            }

            /* ---------------------------------------------------------------- */
            /* classification of k                                             */
            if (sum < cur_k) {                      /* deficient – not semiperfect      */
                ++cur_k;
                continue;
            }

            seq_t answer;
            if (sum == cur_k) {                     /* perfect number                  */
                answer = (seq_t)divcnt;
                found = true;
            } else {                                /* abundant – need subset sum       */
                size_t diff = (size_t)(sum - cur_k);
                if (dpdiff_cap < diff + 1) {
                    int *new_arr = (int *)realloc(dpdiff, (diff + 1) * sizeof(int));
                    if (!new_arr) exit(1);
                    dpdiff = new_arr;
                    dpdiff_cap = diff + 1;
                }
                for (size_t i = 0; i <= diff; ++i) dpdiff[i] = INF;
                dpdiff[0] = 0;

                for (int i = 0; i < divcnt; ++i) {
                    int d = (int)divs[i];
                    if ((size_t)d > diff) continue;
                    for (size_t s = diff; s >= (size_t)d; --s)
                        if (dpdiff[s - d] + 1 < dpdiff[s])
                            dpdiff[s] = dpdiff[s - d] + 1;
                }

                if (dpdiff[diff] < INF) {
                    answer = (seq_t)(divcnt - dpdiff[diff]);
                    found = true;
                } else {                              /* weird number – not semiperfect   */
                    ++cur_k;
                    continue;
                }
            }

            /* ---------------------------------------------------------------- */
            /* store the computed value                                          */
            if (computed_n == values_cap) {
                size_t new_cap = values_cap ? values_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                values = tmp;
                values_cap = new_cap;
            }
            values[computed_n++] = answer;
        }
        /* move to the next integer after the one just processed                */
        ++cur_k;
    }

    return values[n - 1];
}
