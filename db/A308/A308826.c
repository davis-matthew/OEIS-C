/*
 * A308826  Sum of the second largest parts in the partitions of n into 5
 *          parts.
 * https://oeis.org/A308826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of the second largest parts in the partitions of n
 *            into 5 parts. The algorithm uses a dynamic‑programming
 *            construction of all solutions of t1+2t2+3t3+4t4 = s. For each s
 *            it stores the number of solutions cnt[s] and the sums of t2, t3,
 *            t4 in sum2[s], sum3[s], sum4[s]. The required sum f...
 * Range:     u8 n <= 18, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5) {
        return 0;
    }

    /* maximum value of s = n - 5b with b = 1..⌊n/5⌋ is n-5 */
    idx_t Smax = n - 5;

    /* allocate tables 0 … Smax */
    seq_t *cnt  = (seq_t *)malloc((size_t)(Smax + 1) * sizeof(seq_t));
    seq_t *sum2 = (seq_t *)malloc((size_t)(Smax + 1) * sizeof(seq_t));
    seq_t *sum3 = (seq_t *)malloc((size_t)(Smax + 1) * sizeof(seq_t));
    seq_t *sum4 = (seq_t *)malloc((size_t)(Smax + 1) * sizeof(seq_t));

    if (!cnt || !sum2 || !sum3 || !sum4) {
        free(cnt); free(sum2); free(sum3); free(sum4);
        return 0;
    }

    /* initialise */
    for (idx_t i = 0; i <= Smax; ++i) {
        cnt[i]  = 0;
        sum2[i] = 0;
        sum3[i] = 0;
        sum4[i] = 0;
    }
    cnt[0] = 1;   /* one way to make 0 with no coins */

    /* coin 1 (weight 1) – only count changes */
    for (idx_t s = 1; s <= Smax; ++s) {
        cnt[s] += cnt[s - 1];
    }

    /* coin 2 (weight 2) – contributes to t2 */
    for (idx_t s = 2; s <= Smax; ++s) {
        seq_t prev_cnt  = cnt[s - 2];
        seq_t prev_sum2 = sum2[s - 2];
        seq_t prev_sum3 = sum3[s - 2];
        seq_t prev_sum4 = sum4[s - 2];

        cnt[s]  += prev_cnt;
        sum2[s] += prev_sum2 + prev_cnt;   /* t2 increases by 1 */
        sum3[s] += prev_sum3;
        sum4[s] += prev_sum4;
    }

    /* coin 3 (weight 3) – contributes to t3 */
    for (idx_t s = 3; s <= Smax; ++s) {
        seq_t prev_cnt  = cnt[s - 3];
        seq_t prev_sum2 = sum2[s - 3];
        seq_t prev_sum3 = sum3[s - 3];
        seq_t prev_sum4 = sum4[s - 3];

        cnt[s]  += prev_cnt;
        sum2[s] += prev_sum2;
        sum3[s] += prev_sum3 + prev_cnt;   /* t3 increases by 1 */
        sum4[s] += prev_sum4;
    }

    /* coin 4 (weight 4) – contributes to t4 */
    for (idx_t s = 4; s <= Smax; ++s) {
        seq_t prev_cnt  = cnt[s - 4];
        seq_t prev_sum2 = sum2[s - 4];
        seq_t prev_sum3 = sum3[s - 4];
        seq_t prev_sum4 = sum4[s - 4];

        cnt[s]  += prev_cnt;
        sum2[s] += prev_sum2;
        sum3[s] += prev_sum3;
        sum4[s] += prev_sum4 + prev_cnt;   /* t4 increases by 1 */
    }

    /* accumulate the final result */
    seq_t total = 0;
    for (idx_t b = 1; b <= n / 5; ++b) {
        idx_t s = n - 5 * b;
        total += (seq_t)b * cnt[s] + sum2[s] + sum3[s] + sum4[s];
    }

    free(cnt);
    free(sum2);
    free(sum3);
    free(sum4);

    return total;
}
