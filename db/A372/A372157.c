/*
 * A372157  E.g.f. A(x) satisfies A(x) = exp( 3 * x * (1 + x * A(x)^(1/3)) ).
 * https://oeis.org/A372157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A372157: a(n) = 3 * n! * Σ_{m=0}^{⌊n/2⌋} (m+3)^{n-m-1} / (m! (n-2m)!)
 *
 * The function uses an incremental factorial cache to avoid recomputation.
 * Each term is computed as: coeff * (m+3)^(n-m-1),
 * where coeff = 3 * n! / (m! (n-2m)!).
 */

#include <stdlib.h>          /* malloc/realloc */
#include "../utils/seq_pow.h"/* seq_pow(seq_t, idx_t) */

static seq_t *fact = NULL;   /* factorial cache */
static idx_t fact_cap = 0;   /* current capacity of fact array */

/* Ensure factorial array contains fact[0]..fact[n] */
static void ensure_factorial_up_to(idx_t n)
{
    if (fact_cap > n) return;

    idx_t new_cap = n + 1;
    seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
    if (!new_fact) return; /* allocation failed; caller will use whatever is available */

    /* Compute missing factorials */
    for (idx_t i = fact_cap; i <= n; ++i) {
        if (i == 0) new_fact[i] = 1;
        else new_fact[i] = new_fact[i - 1] * (seq_t)i;
    }

    fact = new_fact;
    fact_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1; /* a(0) = 1 */

    ensure_factorial_up_to(n);

    idx_t max_m = n / 2;      /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t m = 0; m <= max_m; ++m) {
        /* exponent for the power term */
        idx_t exp = n - m - 1;                     /* >= 0 because n >= 1 */

        /* power = (m+3)^(n-m-1) */
        seq_t power = seq_pow((seq_t)(m + 3), exp);

        /* coeff = 3 * n! / (m! (n-2m)!) */
        seq_t coeff = fact[n];
        coeff /= fact[m];
        coeff /= fact[n - 2 * m];
        coeff *= (seq_t)3;

        /* term = coeff * power */
        seq_t term = coeff * power;
        sum += term;
    }

    return sum;
}
