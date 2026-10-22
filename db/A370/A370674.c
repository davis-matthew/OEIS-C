/*
 * A370674  Expansion of Sum_{k>0} k! * x^k/(1+x^k)^(k+1).
 * https://oeis.org/A370674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * (d + n/d - 1)! / (d-1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache of factorials: fact[i] = i! for i = 0..fact_len-1 */
static seq_t *fact = NULL;
static idx_t  fact_len = 0;   /* number of precomputed factorials */

/* Ensure factorials up to n are available. */
static void ensure_factorial(idx_t n)
{
    if (n + 1 <= fact_len)
        return;  /* already computed */

    /* Grow the factorial array to size n+1 */
    idx_t new_len = n + 1;
    seq_t *new_fact = NULL;

    if (fact == NULL) {
        new_fact = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        new_fact = (seq_t *)realloc(fact, new_len * sizeof(seq_t));
    }

    if (new_fact == NULL)
        return;  /* allocation failed: leave array unchanged */

    /* Initialize missing entries */
    if (fact_len == 0) {           /* first allocation */
        new_fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
    } else {                       /* extend existing array */
        for (idx_t i = fact_len; i <= n; ++i)
            new_fact[i] = new_fact[i - 1] * (seq_t)i;
    }

    fact = new_fact;
    fact_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n < 1 */

    /* Pre‑compute factorials up to n */
    ensure_factorial(n);

    seq_t sum_pos = (seq_t)0;
    seq_t sum_neg = (seq_t)0;

    /* Enumerate all divisors d of n (half the loop, using symmetry) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t d2 = n / d;

        /* divisor d */
        idx_t e   = d + d2 - 1;                         /* d + n/d - 1 */
        seq_t term = fact[e] / fact[d - 1];            /* (d + n/d - 1)! / (d-1)! */
        if ((d & 1U) == 1)
            sum_pos += term;            /* (-1)^(d-1) = +1 for odd d */
        else
            sum_neg += term;            /* (-1)^(d-1) = -1 for even d */

        /* paired divisor d2 if distinct */
        if (d2 != d) {
            idx_t e2   = d2 + d - 1;                     /* same as d2 + n/d2 - 1 */
            seq_t term2 = fact[e2] / fact[d2 - 1];
            if ((d2 & 1U) == 1)
                sum_pos += term2;
            else
                sum_neg += term2;
        }
    }

    /* Result is the difference of the two sums */
    return sum_pos - sum_neg;
}
