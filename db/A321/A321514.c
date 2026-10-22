/*
 * A321514  Number of ways to choose a factorization of each integer from 2 to
 *          n into factors > 1.
 * https://oeis.org/A321514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} A001055(k) (product of number of
 *            multiplicative partitions)
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   _count_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_count_factor.h"

/* forward declaration of the recursive helper */
static seq_t _count_factor(idx_t n, idx_t min);

/* state kept between calls */
static seq_t *f   = NULL;       /* f[i] = A001055(i)          */
static seq_t *prod = NULL;      /* prod[i] = product up to i   */
static idx_t cap   = 0;         /* current maximum index stored */

/* public entry point */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset is 1; n=0 is undefined – return 0 for safety */
        return (seq_t)0;

    /* grow the cache if necessary */
    if (n > cap)
    {
        idx_t old = cap;
        cap = n;
        f   = (seq_t*)realloc(f,   sizeof(seq_t) * (cap + 1));
        prod = (seq_t*)realloc(prod, sizeof(seq_t) * (cap + 1));
        if (!f || !prod) { /* memory failure – abort */
            exit(EXIT_FAILURE);
        }

        if (old == 0)
        {
            f[0]      = (seq_t)0;      /* unused for idx = 0 */
            prod[0]   = (seq_t)1;      /* product of zero terms */
            f[1]      = (seq_t)1;      /* by convention */
            prod[1]   = prod[0] * f[1];
            old = 1;
        }

        /* compute new entries */
        for (idx_t i = old + 1; i <= cap; ++i)
        {
            if (i == 1)
            {
                f[1] = (seq_t)1;
            }
            else
            {
                f[i] = _count_factor(i, (idx_t)2);
            }
            prod[i] = prod[i - 1] * f[i];
        }
    }

    return prod[n];
}
