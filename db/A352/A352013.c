/*
 * A352013  a(n) = Sum_{d|n} (-1)^(n/d+1) * (n-1)!/(d-1)!.
 * https://oeis.org/A352013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * (n-1)!/(d-1)!
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for the products (n-1)!/(d-1)! for the current n */
static seq_t *prod_table = NULL;
static idx_t prod_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, undefined for n==0 */
    if (n == 1) return 1;          /* a(1) = 1 */

    /* resize cache if needed */
    if (n > prod_cap) {
        free(prod_table);
        prod_table = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        prod_cap = n;
    }

    /* prod_table[d] = ∏_{i=d}^{n-1} i  (that is (n-1)!/(d-1)!) */
    prod_table[n] = 1;
    for (idx_t i = n - 1; i > 0; --i) {
        prod_table[i] = prod_table[i + 1] * (seq_t)i;
    }

    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            idx_t d = i;
            idx_t k = n / d;
            seq_t term = prod_table[d];
            if ((k & 1) == 0) term = -term;
            sum += term;

            /* second divisor d = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d) {
                d = d2;
                k = n / d;
                term = prod_table[d];
                if ((k & 1) == 0) term = -term;
                sum += term;
            }
        }
    }

    return sum;
}
