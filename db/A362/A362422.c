/*
 * A362422  Number of partitions of n into 2 perfect powers (A001597).
 * https://oeis.org/A362422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (x,y) of perfect powers such
 *            that x + y = n (x ≤ y)
 * Range:     u8 n <= 15, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 91 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* static cache of perfect powers */
    static seq_t *pp = NULL;
    static size_t pp_sz = 0;
    static idx_t pp_max = 0;

    /* rebuild cache if we need larger values */
    if (n > pp_max) {
        if (pp) mem_free(pp);
        /* temporary buffer for generating perfect powers */
        size_t tmp_cap = (size_t)n + 1;
        seq_t *tmp = mem_new(seq_t, tmp_cap);
        if (!tmp) return 0;            /* allocation failure --> zero */
        size_t tmp_sz = 0;

        /* 1 is a perfect power */
        if (n >= 1) {
            tmp[tmp_sz++] = 1;
        }

        /* generate a^b for b ≥ 2 */
        for (idx_t base = 2; base <= n; ++base) {
            seq_t val = base * base;
            while (val <= (seq_t)n) {
                if (tmp_sz == tmp_cap) {
                    tmp_cap *= 2;
                    tmp = mem_resize(seq_t, tmp, tmp_cap);
                    if (!tmp) return 0;
                }
                tmp[tmp_sz++] = val;
                val *= base;
            }
        }

        /* sort */
        mem_qsort(tmp, tmp_sz, sizeof(seq_t), seq_cmp);

        /* unique (deduplicate) */
        size_t uniq_sz = 0;
        for (size_t i = 0; i < tmp_sz; ++i) {
            if (uniq_sz == 0 || tmp[i] != tmp[uniq_sz - 1]) {
                tmp[uniq_sz++] = tmp[i];
            }
        }

        pp      = tmp;
        pp_sz   = uniq_sz;
        pp_max  = n;
    }

    if (n < 2) return 0;  /* no partitions for n = 0 or 1 */

    seq_t count = 0;
    seq_t n_t   = (seq_t)n;

    /* iterate over perfect powers ≤ n/2 */
    for (size_t i = 0; i < pp_sz; ++i) {
        seq_t a_val = pp[i];
        if (a_val > n_t / 2) break;        /* ensure a ≤ b */
        seq_t b_val = n_t - a_val;

        /* binary search for b_val in the list */
        if (bsearch(&b_val, pp, pp_sz, sizeof(seq_t), seq_cmp)) {
            ++count;
        }
    }

    return count;
}
