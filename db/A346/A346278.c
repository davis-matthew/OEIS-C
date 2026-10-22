/*
 * A346278  Numbers that are the sum of seven fifth powers in exactly one way.
 * https://oeis.org/A346278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Computation: generate all sums of seven fifth powers of positive integers
   up to a certain bound, then keep those sums that occur exactly once.
   The resulting sequence is sorted ascending; a(n) returns the nth term
   (1‑based indexing).
*/

#include <stdlib.h>

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* combinatorial number C(n,k) (k=7 here) */
static size_t comb(size_t n, size_t k)
{
    size_t res = 1;
    for (size_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* recursive enumeration of all non‑decreasing 7‑tuples (indices) */
static void rec_gen(int pos, int start, seq_t cur, seq_t *out,
                    size_t *idx, const seq_t *pow5, int M)
{
    if (pos == 7) {
        out[*idx] = cur;
        ++(*idx);
        return;
    }
    for (int i = start; i < M; ++i)
        rec_gen(pos + 1, i, cur + pow5[i], out, idx, pow5, M);
}

/* global state */
static seq_t *global_unique = NULL;
static idx_t global_len   = 0;
static idx_t global_M     = 20; /* initial upper bound on base values */

/* ensure that the first `need` unique sums have been generated */
static void ensure_sequence(idx_t need)
{
    while (global_len < need) {
        size_t num_sums = comb((size_t)global_M + 6, 7);
        seq_t *sums = (seq_t *)malloc(num_sums * sizeof(seq_t));
        if (!sums) abort();

        /* powers of the first `global_M` bases */
        seq_t pow5[32]; /* big enough for any reasonable M */
        for (int i = 0; i < global_M; ++i) {
            seq_t v = (seq_t)(i + 1);
            pow5[i] = v * v * v * v * v;   /* (i+1)^5 */
        }

        size_t sum_idx = 0;
        rec_gen(0, 0, (seq_t)0, sums, &sum_idx, pow5, (int)global_M);

        /* sort all sums */
        qsort(sums, sum_idx, sizeof(seq_t), cmp_seq_t);

        /* extract unique sums */
        seq_t *uniques = (seq_t *)malloc(sum_idx * sizeof(seq_t));
        if (!uniques) abort();
        size_t uniq_cnt = 0;
        for (size_t i = 0; i < sum_idx; ) {
            size_t j = i + 1;
            while (j < sum_idx && sums[j] == sums[i]) ++j;
            if (j == i + 1)   /* appears exactly once */
                uniques[uniq_cnt++] = sums[i];
            i = j;
        }

        free(sums);
        free(global_unique);
        global_unique = uniques;
        global_len    = uniq_cnt;

        /* if still not enough, increase the bound and try again */
        if (global_len < need)
            global_M += 5;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */
    ensure_sequence(n);
    return global_unique[n - 1];
}
