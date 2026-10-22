/*
 * A347439  Number of factorizations of n with integer reciprocal alternating
 *          product.
 * https://oeis.org/A347439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6759, u16 n <= 9975, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   divisors_list
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm for A347439
   Computes the number of weakly increasing factorizations of n
   where the reciprocal alternating product = ∏ y_i^{(-1)^i} is an integer.
   For a factorization y_1,…,y_k (k≥0) the product
        P_odd  = ∏_{i odd} y_i
        P_even = ∏_{i even} y_i
   is integer iff P_even % P_odd == 0.
   The code enumerates all such factorizations recursively. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/divisors_list.h"

/* recursive DFS that enumerates factorizations of 'rem', the next factor must
   be at least 'last'.  'len' is the number of factors chosen so far.
   prod_odd/even keep the products of odd/even positioned factors. */
static void dfs(idx_t rem, idx_t last, idx_t len,
                seq_t prod_odd, seq_t prod_even, seq_t *cnt)
{
    if (rem == 1) {
        if (prod_even % prod_odd == 0)
            (*cnt)++;
        return;
    }

    idx_t divisors[256];
    size_t dcnt = 0;
    divisors_list(rem, divisors, &dcnt);

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t f = divisors[i];
        if (f < last)   /* maintain weakly increasing order */
            continue;

        seq_t new_odd = prod_odd;
        seq_t new_even = prod_even;
        if ((len + 1) & 1)  /* new index is odd (1‑based) */
            new_odd *= (seq_t)f;
        else
            new_even *= (seq_t)f;

        dfs(rem / f, f, len + 1, new_odd, new_even, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;    /* the empty factorization */

    seq_t count = 0;
    dfs(n, 2, 0, (seq_t)1, (seq_t)1, &count);
    return count;
}
