/*
 * A341840  Square array T(n, k), n, k >= 0, read by antidiagonals; for any
 *          number m with runs in binary expansion (r_1, ..., r_j), let R(m) =
 *          {r_1 + ... + r_j, r_2 + ... + r_j, ..., r_j}; T(n, k) is the
 *          unique number t such that R(t) is the intersection of R(n) and of
 *          R(k).
 * https://oeis.org/A341840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, look
 * Formula:   a(n) = T(i, j) where (i, j) are the antidiagonal coordinates of
 *            index n in the array T(n, k). The implementation follows the
 *            definition: - R(m) is the set of suffix sums of the binary
 *            run‑lengths of m. - T(i, j) is the unique number whose R(t)
 *            equals the intersection R(i) ∩ R(j). The sequence a i...
 * Range:     u8 n <= 9877, u16 n <= 9877, u32 n <= 9877, u64 n <= 9877,
 *            u128 n <= 9877, big any n
 * Helpers:   get_suffix_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_suffix_sums.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1U) * (d + 2U) / 2U <= n) ++d;

    idx_t prev = d * (d + 1) / 2;
    idx_t k = n - prev;
    idx_t i = k;
    idx_t j = d - k;

    /* get suffix sums for i and j */
    idx_t suff_i[64], suff_j[64];
    size_t cnt_i = 0, cnt_j = 0;
    get_suffix_sums(i, suff_i, &cnt_i);
    get_suffix_sums(j, suff_j, &cnt_j);

    /* intersection of the two suffix sets */
    idx_t inter[64];
    size_t inter_cnt = 0;
    size_t pi = 0, pj = 0;
    while (pi < cnt_i && pj < cnt_j) {
        if (suff_i[pi] == suff_j[pj]) {
            inter[inter_cnt++] = suff_i[pi];
            ++pi;
            ++pj;
        } else if (suff_i[pi] < suff_j[pj]) {
            ++pi;
        } else {
            ++pj;
        }
    }
    if (inter_cnt == 0) return 0;   /* intersection empty → T(…,…) = 0 */

    /* reverse run lengths: r_j, r_{j-1}, …, r_1 */
    idx_t run_rev[64];
    run_rev[0] = inter[0];
    for (size_t t = 1; t < inter_cnt; ++t) {
        run_rev[t] = inter[t] - inter[t - 1];
    }

    /* reconstruct the number from the run lengths */
    seq_t result = 0;
    bool bit_is_one = true;
    for (int t = (int)inter_cnt - 1; t >= 0; --t) {
        idx_t len = run_rev[t];
        result <<= len;
        if (bit_is_one) {
            seq_t mask;
            if (len >= sizeof(seq_t) * 8U) {
                mask = (seq_t)-1;
            } else {
                mask = (((seq_t)1 << len) - 1);
            }
            result |= mask;
        }
        bit_is_one = !bit_is_one;
    }
    return result;
}
