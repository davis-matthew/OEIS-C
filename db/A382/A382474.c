/*
 * A382474  a(n) = Sum_{k=0..n} binomial(k+7,7) * binomial(2*k,2*n-2*k).
 * https://oeis.org/A382474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 31, u128 n <= 72,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A382474
 * Formula: a(n) = Sum_{k=0..n} binomial(k+7,7) * binomial(2*k,2*n-2*k) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Cache values of binomial(k+7,7) for 0 <= k <= n */
    static seq_t *choose = NULL;
    static idx_t choose_cap = 0;

    if (n + 1 > choose_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) return 0;  /* allocation failure */

        /* copy existing values */
        for (idx_t i = 0; i < choose_cap; ++i)
            new_arr[i] = choose[i];

        /* compute new entries */
        for (idx_t i = choose_cap; i < new_cap; ++i) {
            if (i == 0)
                new_arr[i] = (seq_t)1;  /* C(7,7) */
            else
                new_arr[i] = (new_arr[i - 1] * (seq_t)(i + 7)) / (seq_t)i;
        }

        if (choose)
            free(choose);
        choose     = new_arr;
        choose_cap = new_cap;
    }

    seq_t sum = 0;
    /* start_k is the smallest k such that binomial(2*k,2*n-2*k) is non‑zero */
    idx_t start_k = (n + 1) / 2;  /* ceil(n/2) */
    for (idx_t k = start_k; k <= n; ++k) {
        idx_t two_k = 2 * k;
        idx_t two_n_minus_2k = 2 * n - two_k;
        seq_t term = binomial(two_k, two_n_minus_2k);
        sum += choose[k] * term;
    }
    return sum;
}
