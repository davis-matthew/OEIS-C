/*
 * A261927  Sum of the larger parts of the partitions of n into two squarefree
 *          parts.
 * https://oeis.org/A261927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} (n-i) * mu(i)^2 * mu(n-i)^2 (i.e. sum of
 *            the larger parts of the partitions of n into two square‑free
 *            parts)
 * Range:     u8 n <= 35, u16 n <= 575, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple sieve to keep track of square‑free
   numbers up to the largest n requested, and then sums the larger parts. */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of square‑free flags, indexed by integer values */
static bool *squarefree_cache = NULL;
static idx_t squarefree_size = 0;

/* Ensure that squarefree_cache[0..limit] is initialized */
static void
ensure_squarefree(idx_t limit)
{
    if (limit < squarefree_size)
        return;

    idx_t new_size = limit + 1;

    /* realloc must be casted in C++ */
    bool *new_arr = (bool *)realloc(squarefree_cache,
                                    new_size * sizeof(bool));
    if (!new_arr) {
        exit(EXIT_FAILURE);
    }
    squarefree_cache = new_arr;

    /* Initialise new entries to true (square‑free) */
    for (idx_t i = squarefree_size; i < new_size; ++i) {
        squarefree_cache[i] = true;
    }

    /* Mark multiples of squares of primes as NOT square‑free */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!squarefree_cache[p])   /* skip composites */
            continue;
        idx_t sq = p * p;
        for (idx_t k = sq; k <= limit; k += sq) {
            squarefree_cache[k] = false;
        }
    }

    squarefree_size = new_size;
}

/* Computes a(n) for the OEIS sequence A261927. */
seq_t a(idx_t n)
{
    if (n == 0)               /* defined only for n >= 1 */
        return (seq_t)0;

    ensure_squarefree(n);

    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i) {
        if (squarefree_cache[i] && squarefree_cache[n - i]) {
            sum += (seq_t)(n - i);
        }
    }
    return sum;
}
