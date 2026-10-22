/*
 * A316722  Expansion of Product_{k=1..10} (1+x^(2*k-1))/(1-x^(2*k)).
 * https://oeis.org/A316722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of distinct odd parts
 *            1,3,5,…,19 and unlimited even parts 2,4,6,…,20
 * Range:     u8 n <= 22, u16 n <= 61, u32 n <= 253, u64 n <= 3065,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *coeff = NULL;      /* array of coefficients */
static idx_t  max_comp = 0;       /* largest index computed */

/* Ensure coefficients are computed up to at least n */
static void ensure_computed(idx_t n)
{
    if (coeff && n <= max_comp) return;                /* already done */

    /* Allocate/resize array */
    seq_t *new_coeff = (seq_t*)malloc(sizeof(seq_t) * (n + 1));
    if (!new_coeff) return;                            /* allocation failed */

    /* initialize to zero */
    for (idx_t i = 0; i <= n; ++i) new_coeff[i] = (seq_t)0;
    new_coeff[0] = (seq_t)1;                           /* base case */

    /* Perform the product: for k = 1..10, multiply by (1 + x^(2k-1))
       and then by 1/(1 - x^(2k)), i.e. unlimited usage of even part 2k. */
    for (idx_t k = 1; k <= 10; ++k) {
        idx_t odd  = 2 * k - 1;                        /* odd exponent */
        idx_t even = 2 * k;                            /* even exponent */

        /* multiply by (1 + x^odd) */
        for (idx_t i = n; i >= odd; --i)
            new_coeff[i] = new_coeff[i] + new_coeff[i - odd];

        /* multiply by 1/(1 - x^even): unlimited use of even part */
        for (idx_t i = even; i <= n; ++i)
            new_coeff[i] = new_coeff[i] + new_coeff[i - even];
    }

    /* Replace old array */
    free(coeff);
    coeff = new_coeff;
    max_comp = n;
}

/* Return the nth term of A316722 (offset 0). */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return coeff[n];
}
