/*
 * A303944  Number of partitions of n into at most 1^2 copy of 1, 2^2 copies
 *          of 2, 3^2 copies of 3, ... .
 * https://oeis.org/A303944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A303944 – number of partitions of n into at most k^2 copies of k
 *            for each k>=1; The generating function is: ∏_{k≥1}
 *            (1‑x^{k*(k^2+1)})/(1‑x^k)
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef A303944_H
#define A303944_H

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* External integer types supplied by the build environment. */
extern seq_t a(idx_t n);
extern idx_t _floor_isqrt_u64(idx_t);
extern void ensure_cache(idx_t, seq_t**, size_t*);
extern void free_cache(seq_t*);

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    const size_t N = (size_t)n;

#ifdef __cplusplus
    /* C++ implementation using std::vector for simplicity. */
    std::vector<seq_t> cur(N + 1), nxt(N + 1);
    cur[0] = (seq_t)1;
    for (size_t k = 1; k <= N; ++k) {
        size_t max_copies = k * k;
        if (max_copies > N / k) {
            max_copies = N / k;
        }
        if (max_copies == 0) { continue; }

        std::fill(nxt.begin(), nxt.end(), (seq_t)0);

        for (size_t r = 0; r < k; ++r) {
            seq_t window_sum = (seq_t)0;
            size_t cnt = 0;
            for (size_t s = r; s <= N; s += k) {
                window_sum += cur[s];
                if (cnt > max_copies) {
                    size_t idx = s - (max_copies + 1) * k;
                    window_sum -= cur[idx];
                }
                nxt[s] = window_sum;
                ++cnt;
            }
        }
        cur.swap(nxt);
    }
    return cur[N];
#else
    /* C implementation using malloc/calloc.  seq_t must be
       a trivially constructible integer type. */
    seq_t *cur = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *nxt = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!cur || !nxt) {
        free(cur);
        free(nxt);
        return (seq_t)0;
    }
    cur[0] = (seq_t)1;

    for (size_t k = 1; k <= N; ++k) {
        size_t max_copies = k * k;
        if (max_copies > N / k) {
            max_copies = N / k;
        }
        if (max_copies == 0) { continue; }

        memset(nxt, 0, (N + 1) * sizeof(seq_t));

        for (size_t r = 0; r < k; ++r) {
            seq_t window_sum = (seq_t)0;
            size_t cnt = 0;
            for (size_t s = r; s <= N; s += k) {
                window_sum += cur[s];
                if (cnt > max_copies) {
                    size_t idx = s - (max_copies + 1) * k;
                    window_sum -= cur[idx];
                }
                nxt[s] = window_sum;
                ++cnt;
            }
        }
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    seq_t result = cur[N];
    free(cur);
    free(nxt);
    return result;
#endif
}

#endif /* A303944_H */
