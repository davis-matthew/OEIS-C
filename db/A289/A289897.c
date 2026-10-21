/*
 * A289897  Number of matchings in the n-triangular honeycomb rook graph.
 * https://oeis.org/A289897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} A000085(k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of the A000085 sequence and its cumulative product */
    static seq_t *inv   = NULL;  /* A000085 values, indices 0..n */
    static seq_t *prod  = NULL;  /* cumulative product prod[i] = Prod_{k=1..i} inv[k] */
    static idx_t cur_max = 0;    /* highest index computed so far */
    static idx_t cap     = 0;    /* allocated size of the arrays */

    if (n == 0)                  /* safety for any external call */
        return 1;

    /* grow the arrays if necessary */
    if (cap < n + 1) {
        idx_t   new_cap = n + 1;
        seq_t * new_inv  = (seq_t *)realloc(inv,  new_cap * sizeof(seq_t));
        seq_t * new_prod = (seq_t *)realloc(prod, new_cap * sizeof(seq_t));
        if (!new_inv || !new_prod)        /* allocation failed */
            exit(EXIT_FAILURE);
        inv  = new_inv;
        prod = new_prod;
        cap  = new_cap;
    }

    /* initialise the first two values on first use */
    if (cur_max == 0) {
        inv[0]  = 1;          /* by definition of A000085(0) */
        inv[1]  = 1;          /* a(1) = 1 */
        prod[0] = 1;          /* product of an empty set */
        prod[1] = 1;          /* product up to 1 is 1 */
        cur_max = 1;
    }

    /* compute new entries up to n */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        /* recurrence for A000085: a(i) = a(i-1) + (i-1)*a(i-2) */
        seq_t term = inv[i - 1] + (seq_t)(i - 1) * inv[i - 2];
        inv[i]   = term;
        prod[i]  = prod[i - 1] * term;   /* cumulative product */
    }

    cur_max = n;
    return prod[n];
}
