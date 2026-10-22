/*
 * A371208  Number of minimum vertex colorings in the complement of the wheel
 *          graph on n nodes.
 * https://oeis.org/A371208
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*((n-1)/2)! for n odd; a(4) = 1; a(n) =
 *            (1/4)*(n-1)*n^2*(n/2-1)! for n even and n>4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact = NULL;
static size_t fact_cap = 0;

static seq_t factorial_of_idx(idx_t m)
{
    if (fact == NULL) {
        /* initialize 0! */
        fact_cap = 1;
        fact = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        if (!fact) return 0;        /* allocation failure */
        fact[0] = (seq_t)1;
    }
    if (fact_cap <= (size_t)m) {
        size_t new_cap = (size_t)m + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure */
        fact = tmp;
        for (size_t i = fact_cap; i < new_cap; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }
    return fact[(size_t)m];
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;   /* undefined for n < 4 */

    if (n == 4) return (seq_t)1;

    if ((n & 1) == 1) {  /* odd */
        idx_t m = (n - 1) / 2;
        seq_t f = factorial_of_idx(m);
        return (seq_t)(n - 1) * f;
    } else { /* even and n > 4 */
        idx_t m = n / 2 - 1;
        seq_t f = factorial_of_idx(m);
        seq_t ans = f * (seq_t)(n - 1);
        ans = ans * (seq_t)n;
        ans = ans * (seq_t)n;
        ans = ans / 4;
        return ans;
    }
}
