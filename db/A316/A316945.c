/*
 * A316945  A triple of positive integers (n,p,k) is admissible if there exist
 *          at least two different multisets of k positive integers,
 *          {x_1,x_2,...,x_k} and {y_1,y_2,...,y_k}, such that
 *          x_1+x_2+...+x_k=y_1+y_2+...+y_k = n and x_1x_2...x_k =
 *          y_1y_2...y_k = p. For each n, let A(n)={k:(n,p,k) is admissible
 *          for...
 * https://oeis.org/A316945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = n-10 for n ≥ 19; 
   for n < 19, a(n) is obtained by brute‑force enumeration of
   all partitions of n and counting k-values that occur with
   at least two distinct products. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Partition element: number of parts (k) and product of parts (p) */
typedef struct {
    idx_t k;   /* number of parts, always ≤ n ≤ 18 in this branch */
    seq_t p;   /* product of parts */
} part_t;

/* Comparison function for qsort: first by k, then by p */
static int cmp_parts(const void *a, const void *b)
{
    const part_t *pa = (const part_t *)a;
    const part_t *pb = (const part_t *)b;
    if (pa->k < pb->k) return -1;
    if (pa->k > pb->k) return 1;
    if (pa->p < pb->p) return -1;
    if (pa->p > pb->p) return 1;
    return 0;
}

/* Recursive enumeration of partitions of 'rem' with largest part ≤ 'maxv'.
   Accumulates all (k,product) pairs into 'arr', updating '*cnt'. */
static void
gen_parts(idx_t rem, idx_t maxv, idx_t k, seq_t prod,
          part_t *arr, int *cnt)
{
    if (rem == 0) {
        arr[*cnt].k = k;
        arr[*cnt].p = prod;
        (*cnt)++;
        return;
    }
    for (idx_t v = 1; v <= (rem < maxv ? rem : maxv); ++v) {
        gen_parts(rem - v, v, k + 1, prod * (seq_t)v, arr, cnt);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 19) {
        /* Brute‑force for n < 19 */
        part_t parts[400];            /* enough for p(18) = 385 */
        int part_cnt = 0;

        gen_parts(n, n, (idx_t)0, (seq_t)1, parts, &part_cnt);

        /* Order to group equal (k,p) pairs */
        qsort(parts, part_cnt, sizeof(part_t), cmp_parts);

        /* Mark admissible k values */
        bool admissible[20] = {false};
        int i = 0;
        while (i < part_cnt) {
            int j = i + 1;
            while (j < part_cnt &&
                   parts[j].k == parts[i].k &&
                   parts[j].p == parts[i].p)
                ++j;
            if (j - i >= 2)                  /* at least two partitions */
                admissible[(size_t)parts[i].k] = true;
            i = j;
        }

        /* Count admissible k values (k ≥ 1) */
        seq_t count = 0;
        for (idx_t k = 1; k <= n; ++k)
            if (admissible[(size_t)k])
                count++;
        return count;
    } else {
        /* Closed form for n ≥ 19 */
        return (seq_t)(n - 10);
    }
}
