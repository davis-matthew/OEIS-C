/*
 * A339955  Numbers that are the sum of an odd square s and an even square t
 *          such that 0 < s < t.
 * https://oeis.org/A339955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the increasing list of distinct sums
   of an odd square s and an even square t with 0 < s < t. */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* Comparator for qsort (used only in plain C). */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for offset */
    idx_t target = n;                         /* number of terms needed */
    idx_t limit  = 3;                         /* initial upper bound for t (even) */
    /* increase limit until limit^2/4 is definitely ≥ n, then pad a bit */
    while (limit * limit < target * 4) limit++;
    limit += 10;                              /* safety margin */

    /* Generate candidates repeatedly until we have enough unique values */
    for (;;) {
#ifdef __cplusplus
        std::vector<seq_t> vec;
        vec.reserve(target * 2);              /* rough guess for capacity */
#else
        size_t cap = target * 2 > 8 ? target * 2 : 8;
        seq_t *vec = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!vec) return 0;                    /* allocation failure */
        size_t len = 0;
#endif
        idx_t max_seq_val = (idx_t)((seq_t)~((seq_t)0)); /* largest value representable in seq_t */

        /* Generate all sums odd^2 + even^2 with odd < even ≤ limit */
        for (idx_t t_even = 2; t_even <= limit; t_even += 2) {
            idx_t t_sq = t_even * t_even;
            for (idx_t m = 0; m < t_even / 2; ++m) {
                idx_t r = 2 * m + 1;
                idx_t r_sq = r * r;
                idx_t sum = t_sq + r_sq;
                if (sum <= max_seq_val) {
#ifdef __cplusplus
                    vec.push_back((seq_t)sum);
#else
                    if (len == cap) {                  /* grow buffer */
                        cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(vec, cap * sizeof(seq_t));
                        if (!tmp) { free(vec); return 0; }
                        vec = tmp;
                    }
                    vec[len++] = (seq_t)sum;
#endif
                }
            }
        }

#ifdef __cplusplus
        std::sort(vec.begin(), vec.end());
        auto last = std::unique(vec.begin(), vec.end());
        size_t uniq_len = static_cast<size_t>(last - vec.begin());
        if (uniq_len >= target) {
            seq_t result = vec[target - 1];
            return result;
        }
        /* insufficient unique terms – increase limit and repeat */
#else
        if (len == 0) { free(vec); return 0; }
        qsort(vec, len, sizeof(seq_t), cmp_seq);
        /* unique‑filter in place */
        size_t uniq_idx = 0;
        for (size_t i = 0; i < len; ++i) {
            if (i == 0 || vec[i] != vec[i - 1]) {
                if (uniq_idx != i) vec[uniq_idx] = vec[i];
                ++uniq_idx;
                if (uniq_idx >= target) break;
            }
        }
        seq_t result = vec[target - 1];
        free(vec);
        return result;
#endif
        limit += 10;  /* enlarge search range and try again */
    }
}
