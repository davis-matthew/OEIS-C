/*
 * A347648  Number of partitions of n into at most 2 squarefree parts.
 * https://oeis.org/A347648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into at most 2 squarefree parts. The
 *            function counts the empty partition for n = 0, a single
 *            squarefree part n, and unordered pairs of squarefree summands a
 *            ≤ b with a + b = n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Global cache for square‑free flags.
   squarefree[i] == 1  ⇔  i is squarefree (i > 0).
   squarefree[0] is set to 0 and is never used. */
static uint8_t *squarefree = NULL;  /* 1 = squarefree, 0 = not */
static idx_t squarefree_limit = 0;  /* number of elements stored (size of array) */

/* Extend the squarefree array to cover all indices up to n (inclusive). */
static void extend_squarefree(idx_t n)
{
    /* Allocate a fresh array.  The cast is required for C++ compilation. */
    uint8_t *new_arr = (uint8_t *)malloc((n + 1) * sizeof(*new_arr));
    if (!new_arr) abort(); /* Allocation failure */

    /* Assume all non‑zero numbers are squarefree first. */
    for (idx_t i = 0; i <= n; ++i) new_arr[i] = 1;
    new_arr[0] = 0;  /* 0 is handled specially in a() */

    /* Mark multiples of every square p² as not squarefree. */
    for (idx_t p = 2; p <= n / p; ++p)  /* p ≤ √n safely */
    {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= n; j += sq)
            new_arr[j] = 0;
    }

    /* Replace the old array. */
    free(squarefree);
    squarefree = new_arr;
    squarefree_limit = n + 1;  /* array now contains indices 0 … n */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Special case: the empty partition of 0. */
    if (n == 0) return (seq_t)1;

    /* Ensure the cache covers n. */
    if (n + 1 > squarefree_limit)
        extend_squarefree(n);

    seq_t count = 0;

    /* Partition consisting of n itself (single part) */
    if (squarefree[n]) count++;

    /* Count unordered pairs a ≤ b, a + b = n, both squarefree. */
    for (idx_t a_part = 1; a_part <= n / 2; ++a_part)
    {
        if (squarefree[a_part] && squarefree[n - a_part])
            count++;
    }

    return count;
}
