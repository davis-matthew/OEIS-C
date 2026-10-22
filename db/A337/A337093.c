/*
 * A337093  Difference between the number of unordered factorizations and the
 *          number of distinct sums of terms in these unordered factorizations
 *          for those integers where this difference is positive.
 * https://oeis.org/A337093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001055(m(n)) – A069016(m(n)) where m(n) is the n‑th *
 *            positive integer satisfying A001055(m) > A069016(m). This
 *            implementation * enumerates factorizations of integers on demand
 *            and stores the positive * differences in a growing cache
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: enumerate all unordered factorizations of 'rem'.
 * *count           – number of factorizations found
 * sums[1024], *s_cnt – distinct sums of the factors */
static void dfs(uint64_t rem, uint64_t minf, uint64_t cur_sum,
                seq_t *count, uint64_t *sums, size_t *s_cnt)
{
    for (uint64_t i = minf; i * i <= rem; ++i) {
        if (rem % i == 0) {
            dfs(rem / i, i, cur_sum + i, count, sums, s_cnt);
        }
    }
    /* record current factorization */
    (*count) += 1u;
    uint64_t sum_val = cur_sum + rem;

    /* deduplicate sums */
    size_t k;
    for (k = 0; k < *s_cnt; ++k) {
        if (sums[k] == sum_val)
            return;
    }
    if (*s_cnt < 1024) {
        sums[*s_cnt] = sum_val;
        (*s_cnt)++;
    }
}

seq_t a(idx_t n)
{
    /* cache of positive differences */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t last_m = 1;                   /* last integer examined */

    /* extend cache until we have enough terms */
    while (cache_size < n) {
        seq_t count = 0;
        uint64_t sums[1024];
        size_t sum_cnt = 0;

        dfs((uint64_t)last_m, 2, 0, &count, sums, &sum_cnt);

        seq_t diff = count - (seq_t)sum_cnt;
        if (diff > 0) {
            /* grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – abort */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = diff;
        }
        last_m++;
    }
    return cache[n - 1];
}
