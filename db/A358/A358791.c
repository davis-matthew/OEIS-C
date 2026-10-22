/*
 * A358791  a(n) = n!*Sum_{m=0..floor(n/2)} binomial(n,2*m)^(-1).
 * https://oeis.org/A358791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m=0}^{⌊n/2⌋} (2m)! * (n-2m)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* keep a growing cache of factorials */
    static seq_t *fact = NULL;
    static size_t fact_size = 0; /* number of factorials stored (needs n+1 values) */

    /* extend factorial table if necessary */
    if (fact_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = NULL;
        if (fact == NULL) {
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
        }
        if (tmp == NULL) {
            /* allocation failed – return 0 (unlikely in normal usage) */
            return 0;
        }
        /* initialize new entries */
        if (fact_size == 0) {
            tmp[0] = (seq_t)1;
            fact_size = 1;
        }
        for (size_t i = fact_size; i <= n; ++i) {
            /* factorial[i] = factorial[i-1] * i */
            tmp[i] = tmp[i - 1] * (seq_t)i;
        }
        fact = tmp;
        fact_size = new_size;
    }

    /* compute the sum */
    seq_t result = 0;
    idx_t limit = n / 2; /* floor(n/2) */
    for (idx_t m = 0; m <= limit; ++m) {
        seq_t term = fact[2 * m] * fact[n - 2 * m];
        result += term;
    }
    return result;
}
