/*
 * A294242  Number of partitions of 2n into two parts with the larger part
 *          nonsquarefree.
 * https://oeis.org/A294242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1..n} (1 - mu(2n-i)^2) = number of non‑squarefree
 *            integers t with n ≤ t ≤ 2n-1
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Square‑free cache: 1 = squarefree, 0 = non‑squarefree. */
static unsigned char *squarefree = NULL;
static size_t sf_capacity = 0;

/* Ensure that the cache is valid up to the given index (inclusive). */
static void ensure_squarefree_capacity(size_t limit)
{
    if (limit + 1 <= sf_capacity)
        return;                        /* already enough */

    /* Allocate a new array, initialise everything to 1 (true). */
    size_t new_cap = limit + 1;
    unsigned char *new_sf = (unsigned char *)malloc(new_cap);
    if (!new_sf)                         /* out of memory */
        exit(1);

    for (size_t i = 0; i < new_cap; ++i)
        new_sf[i] = 1;

    /* Mark numbers that are multiples of a square > 1.  */
    /* Only primes up to sqrt(limit) can generate such squares. */
    for (size_t p = 2; p * p <= limit; ++p) {
        /* Quick primality test – the limit is small (≤ 20000 for n ≤ 10000). */
        bool is_prime = true;
        for (size_t d = 2; d * d <= p; ++d) {
            if (p % d == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime)
            continue;

        size_t p2 = p * p;
        for (size_t m = p2; m < new_cap; m += p2)
            new_sf[m] = 0;                 /* not squarefree */
    }

    /* Replace the old cache. */
    free(squarefree);
    squarefree = new_sf;
    sf_capacity = new_cap;
}

/* Computes a(n) for n ≥ 1.  Behaviour for n = 0 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* safety; not used for the sequence */

    /* We need values up to 2n-1. */
    ensure_squarefree_capacity((size_t)(2 * (size_t)n - 1));

    size_t start = (size_t)n;                 /* inclusive lower bound */
    size_t end   = (size_t)(2 * (size_t)n - 1);/* inclusive upper bound */

    seq_t count = 0;
    for (size_t t = start; t <= end; ++t) {
        if (squarefree[t] == 0)              /* not squarefree */
            count += 1;
    }
    return count;
}
