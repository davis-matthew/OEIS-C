/*
 * A261985  Sum of the smaller parts of the partitions of n into two
 *          squarefree parts.
 * https://oeis.org/A261985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i * mu(i)^2 * mu(n-i)^2
 * Range:     u8 n <= 67, u16 n <= 1007, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by summing i when both i and n-i are square‑free. */

#include <stdlib.h>

/* Static cache of squarefree flags (1 = squarefree, 0 = not squarefree) */
static unsigned char *sqfree = NULL;   /* array 0..max_n */
static idx_t max_n = 0;

/* Ensure sqfree[0..n] is initialized; recompute all values if n increased. */
static void init_sqfree(idx_t n)
{
    if (n <= max_n)
        return;                        /* already initialized */

    /* Reallocate array to fit indices up to n */
    unsigned char *tmp = (unsigned char *)realloc(sqfree, (n + 1) * sizeof(unsigned char));
    if (!tmp)
        abort();                       /* allocation failed */
    sqfree = tmp;

    /* Mark all entries as squarefree initially (except 0) */
    for (idx_t i = 0; i <= n; ++i)
        sqfree[i] = 1;
    if (n >= 0)
        sqfree[0] = 0;

    /* For each prime p, mark multiples of p² as not squarefree */
    for (idx_t p = 2; p <= n / p; ++p) {   /* avoid overflow on p*p */
        idx_t sq = p * p;
        for (idx_t k = sq; k <= n; k += sq)
            sqfree[k] = 0;
    }

    max_n = n;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* a(0) undefined, return 0 */

    init_sqfree(n);

    seq_t sum = 0;
    idx_t half = n >> 1;           /* floor(n/2) */

    for (idx_t i = 1; i <= half; ++i) {
        if (sqfree[i] && sqfree[n - i])
            sum += (seq_t)i;
    }
    return sum;
}
