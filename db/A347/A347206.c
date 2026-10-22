/*
 * A347206  The number of overpartitions of n whose Frobenius symbols have
 *          only odd parts in the top row.
 * https://oeis.org/A347206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Product_{k>=1} 1/(1-q^k))*Sum_{m>=0}
 *            q^(m*(3*m+1)/2)*(1-q^(2*m+1))
 * Range:     u8 n <= 17, u16 n <= 45, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"    /* seq_t *seq_alloc(size_t cap) */
#include "../utils/seq_realloc.h" /* seq_t *seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

static seq_t *p_arr = NULL;   /* array of partition numbers p[0..p_max] */
static size_t p_cap = 0;      /* allocated capacity of p_arr */
static idx_t  p_max = 0;      /* largest index currently computed */

/* Ensure p_arr contains p[0..N] computed with the pentagonal recurrence */
static void ensure_partitions(idx_t N)
{
    if (N <= p_max) return;

    size_t needed = (size_t)N + 1;
    if (p_arr == NULL) {
        p_cap = needed > 1 ? needed : 1;
        p_arr = seq_alloc(p_cap);
        if (!p_arr) return;               /* allocation failed */
        for (size_t i = 0; i < p_cap; ++i) p_arr[i] = (seq_t)0;
        p_arr[0] = (seq_t)1;               /* p(0) = 1 */
    } else if (needed > p_cap) {
        size_t new_cap = p_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = seq_realloc(p_arr, p_cap, new_cap);
        if (!tmp) return;                  /* reallocation failed */
        for (size_t i = p_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        p_arr = tmp;
        p_cap = new_cap;
    }

    for (idx_t m = p_max + 1; m <= N; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m) break;

            /* sign alternates with k */
            if (k & 1) sum += p_arr[m - pent1];
            else       sum -= p_arr[m - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > m) continue;
            if (k & 1) sum += p_arr[m - pent2];
            else       sum -= p_arr[m - pent2];
        }
        p_arr[m] = sum;
    }
    p_max = N;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;   /* a(0) = 1 */

    ensure_partitions(n);

    seq_t result = (seq_t)0;
    for (idx_t k = 0; ; ++k) {
        idx_t r1 = k * (3 * k + 1) / 2;
        if (r1 > n) break;
        result += p_arr[n - r1];

        idx_t r2 = r1 + 2 * k + 1;
        if (r2 <= n) result -= p_arr[n - r2];
    }
    return result;
}
