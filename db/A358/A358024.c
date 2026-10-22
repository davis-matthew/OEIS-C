/*
 * A358024  Number of partitions of n into at most 3 distinct squarefree
 *          parts.
 * https://oeis.org/A358024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into at most 3 distinct squarefree
 *            parts
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *sqfree_flag = NULL;
static idx_t *sqfree_list   = NULL;
static size_t  sqfree_size  = 0;
static idx_t   max_precomp  = 0;

/* Precompute squarefree numbers up to n */
static void ensure_precomputed(idx_t n)
{
    if (n <= max_precomp) return;

    /* Allocate flag array: 1 == squarefree, 0 == not */
    unsigned char *flag = (unsigned char*)malloc(((size_t)n + 1) * sizeof(unsigned char));
    if (!flag) return;            /* allocation failed – cannot proceed */

    for (idx_t i = 0; i <= n; ++i) flag[i] = 1;
    flag[0] = 0;

    /* Mark numbers that have a square factor */
    for (idx_t p = 2; p * p <= n; ++p) {
        idx_t sq = p * p;
        for (idx_t m = sq; m <= n; m += sq) flag[m] = 0;
    }

    /* Count squarefree numbers */
    size_t count = 0;
    for (idx_t i = 1; i <= n; ++i) if (flag[i]) ++count;

    /* Build list of squarefree numbers */
    idx_t *list = (idx_t*)malloc(count * sizeof(idx_t));
    if (!list) { free(flag); return; }

    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i) if (flag[i]) list[pos++] = i;

    /* Replace old data with the new one */
    free(sqfree_flag);
    free(sqfree_list);
    sqfree_flag   = flag;
    sqfree_list   = list;
    sqfree_size   = count;
    max_precomp   = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* empty partition */

    ensure_precomputed(n);

    seq_t res = 0;

    /* 1-partitions: single squarefree number equal to n */
    if (sqfree_flag[n]) ++res;

    /* 2-partitions: unordered distinct squarefree pairs */
    for (size_t idx = 0; idx < sqfree_size; ++idx) {
        idx_t a_val = sqfree_list[idx];
        if (a_val >= n) break;
        idx_t b = n - a_val;
        if (b > a_val && b <= max_precomp && sqfree_flag[b]) ++res;
    }

    /* 3-partitions: unordered distinct triplets */
    for (size_t i = 0; i < sqfree_size; ++i) {
        idx_t a_val = sqfree_list[i];
        if (a_val >= n) break;
        idx_t target = n - a_val;
        size_t l = i + 1;
        size_t r = sqfree_size - 1;
        while (l < r) {
            idx_t sum = sqfree_list[l] + sqfree_list[r];
            if (sum == target) {
                ++res;
                ++l;
                --r;
            } else if (sum < target) {
                ++l;
            } else {
                --r;
            }
        }
    }

    return res;
}
