/*
 * A298734  a(n) = n-th term in periodic sequence repeating the divisors of n
 *          in decreasing order.
 * https://oeis.org/A298734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the divisor of n that occupies index ((n-1) mod d(n)) in
 *            the list of divisors of n sorted in decreasing order, where d(n)
 *            is the number of divisors of n
 * Range:     u8 n <= 256, u16 n <= 9128, u32 n <= 9128, u64 n <= 9128,
 *            u128 n <= 9128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_idx(const void *a, const void *b)
{
    idx_t x = *(const idx_t *)a;
    idx_t y = *(const idx_t *)b;
    return (x > y) - (x < y);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0 */

    /* Count number of divisors */
    size_t tau = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            ++tau;                  /* divisor d itself */
            if (d != n / d) ++tau;  /* paired divisor n/d */
        }
    }

    if (tau == 0) return (seq_t)0;   /* should not happen for n >= 1 */

    /* Allocate array for all divisors */
    idx_t *divs = (idx_t *)malloc(tau * sizeof(idx_t));
    if (!divs) return (seq_t)0;       /* allocation failure */

    /* Collect divisors */
    size_t pos = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            divs[pos++] = d;            /* smaller divisor */
            if (d != n / d)
                divs[pos++] = n / d;   /* larger divisor */
        }
    }

    /* Sort ascending order */
    qsort(divs, tau, sizeof(idx_t), cmp_idx);

    /* Determine the desired divisor */
    size_t r   = (size_t)((n - 1) % tau);   /* 0‑based index in the descending sequence */
    size_t idx = tau - 1 - r;              /* corresponding index in ascending array */

    seq_t result = (seq_t)divs[idx];

    free(divs);
    return result;
}
