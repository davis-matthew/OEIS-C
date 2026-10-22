/*
 * A326546  Sum of the fourth largest parts in the partitions of n into 9
 *          primes.
 * https://oeis.org/A326546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of the fourth largest parts in the partitions of n into 9
 *            primes
 * Range:     u8 n <= 46, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_primes, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* Every 9‑prime partition needs at least 9 × 2 = 18, so 0 before that. */
    if (n < 18) return 0;

    /* Build all primes up to n. */
    size_t prime_cnt = 0;
    idx_t *primes = NULL;
    build_primes(n, &primes, &prime_cnt);
    if (!prime_cnt) return 0;

    const idx_t stride = n + 1;
    const size_t cells  = 10 * stride;

    /* dp_cnt[c][s] : number of ways to pick c primes summing to s
       dp_sum[c][s] : weighted sum of the 4th largest part for those ways   */
    seq_t *cnt  = (seq_t *)calloc(cells, sizeof(seq_t));
    seq_t *sum  = (seq_t *)calloc(cells, sizeof(seq_t));
    if (!cnt || !sum) {
        free(primes);
        return 0;
    }
    cnt[0] = 1; /* one way to pick 0 primes with sum 0 */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        seq_t p = (seq_t)primes[pi];

        seq_t *new_cnt = (seq_t *)calloc(cells, sizeof(seq_t));
        seq_t *new_sum = (seq_t *)calloc(cells, sizeof(seq_t));
        if (!new_cnt || !new_sum) {
            free(cnt); free(sum);
            free(primes);
            return 0;
        }

        for (idx_t c = 0; c <= 9; ++c) {
            idx_t base_row = c * stride;
            for (idx_t s = 0; s <= n; ++s) {
                seq_t base_cnt  = cnt[base_row + s];
                if (!base_cnt) continue;
                seq_t base_sumv = sum[base_row + s];

                /* try adding k copies of prime p (0 <= k <= 9-c) */
                for (uint64_t k = 0; k <= 9 - c; ++k) {
                    idx_t c2   = c + (idx_t)k;
                    idx_t s2   = s + (idx_t)(k * p);
                    if (s2 > n) break;

                    idx_t idx2 = c2 * stride + s2;
                    new_cnt[idx2] += base_cnt;

                    seq_t add;
                    if (c < 6) {
                        if (c2 >= 6) {
                            /* the first of the added copies becomes the 4th largest */
                            add = base_sumv + mul_small(base_cnt, (unsigned int)p);
                        } else {
                            add = base_sumv;
                        }
                    } else {
                        add = base_sumv;
                    }
                    new_sum[idx2] += add;
                }
            }
        }

        free(cnt); free(sum);
        cnt = new_cnt;
        sum = new_sum;
    }

    seq_t result = sum[9 * stride + n];
    free(cnt); free(sum); free(primes);
    return result;
}
