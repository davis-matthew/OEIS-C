/*
 * A254232  Product of Perrin numbers A001608(2) * ... * A001608(n).
 * https://oeis.org/A254232
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{i=2}^n P(i) where P is the Perrin sequence
 *            (P(0)=3, P(1)=0, P(2)=2, P(k)=P(k-2)+P(k-3))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 17, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static storage for Perrin numbers and cumulative products */
    static seq_t *perrs = NULL;     /* Perrin numbers P(i)  */
    static seq_t *prods = NULL;     /* product up to i, i>=2   */
    static idx_t current_max = 0;   /* highest index computed so far */

    /* ensure array capacity for indices up to n */
    if (n + 1 > current_max) {
        idx_t new_cap = n + 1;
        /* allocate or grow the Perrin array */
        if (perrs == NULL) {
            perrs = (seq_t *)malloc(new_cap * sizeof(seq_t));
            prods = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            perrs = (seq_t *)realloc(perrs, new_cap * sizeof(seq_t));
            prods = (seq_t *)realloc(prods, new_cap * sizeof(seq_t));
        }

        /* initialize base Perrin numbers if not yet done */
        if (current_max < 3) {
            perrs[0] = (seq_t)3;
            perrs[1] = (seq_t)0;
            perrs[2] = (seq_t)2;
        }

        /* compute Perrin numbers and products up to n */
        if (current_max < 2) {
            prods[2] = perrs[2];   /* product for n=2 */
        }
        for (idx_t k = current_max + 1; k <= n; ++k) {
            if (k >= 3) {
                perrs[k] = perrs[k-2] + perrs[k-3];
            }
            if (k == 2) {
                prods[k] = perrs[k];
            } else if (k > 2) {
                prods[k] = prods[k-1] * perrs[k];
            }
        }
        current_max = n;
    }

    return prods[n];
}
