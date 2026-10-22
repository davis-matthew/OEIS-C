/*
 * A356915  Number of partitions of n into 4 parts that divide n.
 * https://oeis.org/A356915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered quadruples of positive divisors of n
 *            that sum to n. (OEIS A356915)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort, sorts idx_t values ascending */
static int cmp_idx(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    return (*x < *y) ? -1 : (*x > *y);
}

/* Return the number of partitions of n into 4 positive divisors of n. */
seq_t a(idx_t n)
{
    /* For n < 4 no such partition exists */
    if (n < 4) return (seq_t)0;

    /* Gather all divisors of n */
    size_t cap   = 16;
    size_t dcnt  = 0;
    idx_t *divs  = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return (seq_t)0;               /* allocation failure */

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (dcnt == cap) {
                cap <<= 1;
                divs = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                if (!divs) return (seq_t)0;
            }
            divs[dcnt++] = i;

            idx_t j = n / i;
            if (j != i) {
                if (dcnt == cap) {
                    cap <<= 1;
                    divs = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!divs) return (seq_t)0;
                }
                divs[dcnt++] = j;
            }
        }
    }

    /* Sort divisors for non‑decreasing ordering */
    qsort(divs, dcnt, sizeof(idx_t), cmp_idx);

    seq_t count = 0;

    /* Enumerate a ≤ b ≤ c, deduce d = n - (a+b+c) and check ordering
       and divisibility. */
    for (size_t ii = 0; ii < dcnt; ++ii) {
        idx_t a_val = divs[ii];
        if ((seq_t)a_val > (seq_t)n) break;

        for (size_t jj = ii; jj < dcnt; ++jj) {
            idx_t b_val = divs[jj];
            if ((seq_t)a_val + (seq_t)b_val > (seq_t)n) break;

            for (size_t kk = jj; kk < dcnt; ++kk) {
                idx_t c_val = divs[kk];

                seq_t sum3 = (seq_t)a_val + (seq_t)b_val + (seq_t)c_val;
                if (sum3 > (seq_t)n) break;

                idx_t d_val = (idx_t)((seq_t)n - sum3);
                if (d_val < c_val) continue;            /* enforce a≤b≤c≤d */
                if (n % d_val != 0) continue;           /* d must divide n */

                ++count;                                /* valid quadruple */
            }
        }
    }

    free(divs);
    return count;
}
