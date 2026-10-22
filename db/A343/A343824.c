/*
 * A343824  Sum of the elements in all pairs (d1, d2) of divisors of n such
 *          that d1<=d2, d1|n, d2|n, and d1 + d2 <= n.
 * https://oeis.org/A343824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d1,d2 | n, d1<=d2, d1+d2<=n} (d1 + d2)
 * Range:     u8 n <= 23, u16 n <= 1007, u32 n <= 9670, u64 n <= 9670,
 *            u128 n <= 9670, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_uint64(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* safety: offset starts at 1 */

    /* collect all divisors of n */
    idx_t cap = 16;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return 0;               /* allocation failure (unlikely) */
    size_t len = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            if (len == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!tmp) { free(divs); return 0; }
                divs = tmp;
            }
            divs[len++] = i;
            idx_t j = n / i;
            if (j != i) {
                if (len == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[len++] = j;
            }
        }
    }

    /* sort divisors in ascending order */
    qsort(divs, len, sizeof(idx_t), cmp_uint64);

    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;

    for (size_t i = 0; i < len; ++i) {
        seq_t d1 = (seq_t)divs[i];
        for (size_t j = i; j < len; ++j) {
            seq_t d2 = (seq_t)divs[j];
            seq_t add = d1 + d2;
            if (add <= n_seq) {
                sum += add;
            }
        }
    }

    free(divs);
    return sum;
}
