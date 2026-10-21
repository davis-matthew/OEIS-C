/*
 * A285489  Sum of entries in the second cycles of all permutations of [n].
 * https://oeis.org/A285489
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{t=1}^{n-1} (t! · (n-t-1)! · [C(n-2,t-1)·(n(n+1)/2-1)
 *            + (n+1)·C(n-1,t) – t·C(n,t+1)]) / 2 * (Values are exact for any
 *            integer type used for seq_t and idx_t.)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *fac = NULL;        /* factorial array */
static idx_t fac_cap = 0;        /* current capacity of fac */

/* Ensure factorials up to n are precomputed. */
static void ensure_fact(idx_t n)
{
    if (fac_cap >= n) return;
    idx_t new_cap = (fac_cap == 0) ? 1 : fac_cap * 2;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);            /* out of memory */
    fac = tmp;
    for (idx_t i = fac_cap; i < new_cap; ++i) {
        if (i == 0) fac[i] = (seq_t)1;
        else fac[i] = fac[i-1] * (seq_t)i;
    }
    fac_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined below offset */

    /* Precompute factorials up to n-1 (needed for later). */
    ensure_fact((idx_t)n);

    /* v = n*(n+1)/2 - 1  (sum of all integers from 2 .. n) */
    seq_t v = (seq_t)n * (seq_t)(n+1) / (seq_t)2 - (seq_t)1;

    seq_t result = (seq_t)0;

    /* Initial binomial values for t = 1 */
    seq_t C1 = (seq_t)1;                     /* C(n-2, 0)    */
    seq_t C2 = (seq_t)(n-1);                 /* C(n-1, 1)    */
    seq_t C3 = (seq_t)n * (seq_t)(n-1) / (seq_t)2;   /* C(n, 2)     */

    for (idx_t t = 1; t <= n-1; ++t) {
        /*   f_t = C1 * v + (n+1) * C2 - t * C3   */
        seq_t f_t = C1 * v + ((seq_t)(n+1) * C2) - ((seq_t)t * C3);

        /* term = t! * (n-t-1)! * f_t   */
        seq_t term = fac[t] * fac[n - t - 1] * f_t;

        /* add term/2 to the result (exact division) */
        result += term / (seq_t)2;

        /* Update binomials for the next t (if not last iteration) */
        if (t < n-1) {
            idx_t shift1 = n - 1 - t;                 /* = (n-1)-t   */
            idx_t shift2 = n - t - 1;                 /* = n-(t+1)   */

            C1 = C1 * (seq_t)shift1 / (seq_t)t;
            C2 = C2 * (seq_t)shift1 / (seq_t)(t+1);
            C3 = C3 * (seq_t)shift2 / (seq_t)(t+2);
        }
    }
    return result;
}
