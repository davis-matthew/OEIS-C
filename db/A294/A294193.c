/*
 * A294193  a(n) = sum of integers between n!+1 and (n+1)!.
 * https://oeis.org/A294193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ½ * ( (n+1)! * ((n+1)! + 1) - n! * (n! + 1) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact = NULL;
static idx_t   fact_cap = 0;   /* allocated slots               */
static idx_t   fact_last = 0;  /* highest index factorial computed */

/* Ensure that the factorial array can hold indices up to (needed - 1). */
static void ensure_fact_capacity(idx_t needed)
{
    if (needed <= fact_cap)
        return;

    /* Grow capacity (doubling strategy). */
    idx_t new_cap = fact_cap ? fact_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_ptr;
#if defined(__cplusplus)
    new_ptr = new seq_t[new_cap];
#else
    new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    /* Copy existing values. */
    for (idx_t i = 0; i < fact_cap; ++i)
        new_ptr[i] = fact[i];

    /* Free old storage. */
#if defined(__cplusplus)
    delete [] fact;
#else
    free(fact);
#endif

    fact      = new_ptr;
    fact_cap  = new_cap;

    /* Initialize 0! when this is the first allocation. */
    if (fact_last == 0 && fact_cap >= 1)
        fact[0] = (seq_t)1;
}

/* Compute factorials up to index m (inclusive). */
static void compute_factorials_up_to(idx_t m)
{
    if (m <= fact_last)
        return;

    ensure_fact_capacity(m + 1);

    for (idx_t i = fact_last + 1; i <= m; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_last = m;
}

seq_t a(idx_t n)
{
    /* Ensure factorials for n and n+1 are available. */
    compute_factorials_up_to(n + 1);

    seq_t nf   = fact[n];
    seq_t n1f  = fact[n + 1];

    /* term1 = n1f * (n1f + 1) / 2 */
    seq_t term1 = n1f * (n1f + (seq_t)1);
    term1 /= (seq_t)2;

    /* term2 = nf * (nf + 1) / 2 */
    seq_t term2 = nf * (nf + (seq_t)1);
    term2 /= (seq_t)2;

    return term1 - term2;
}
