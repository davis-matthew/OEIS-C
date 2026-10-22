/*
 * A323497  a(n) = Product_{k=1..n} (binomial(k-1,4) + binomial(n-k,4)).
 * https://oeis.org/A323497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} (binomial(k-1,4) + binomial(n-k,4))
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t *binom4 = NULL;
static idx_t  binom_cap = 0;

static void ensure_binom4(idx_t n)
{
    if (n < binom_cap) return;
    idx_t old_cap = binom_cap;
    idx_t new_cap = n + 1;
    seq_t *new_arr = (seq_t *)realloc(binom4, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);          /* out of memory */
    binom4 = new_arr;
    for (idx_t i = old_cap; i <= n; ++i) {
        if (i < 4)
            binom4[i] = 0;
        else
            binom4[i] = binomial(i, 4);
    }
    binom_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* empty product */

    ensure_binom4(n);

    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = binom4[k-1] + binom4[n-k];
        if (term == 0) return 0;   /* product will be zero */
        prod *= term;
    }
    return prod;
}
