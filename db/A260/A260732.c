/*
 * A260732  a(n) = number of steps needed to reach zero when starting from k =
 *          n^2 and repeatedly applying the map that replaces k with k - {the
 *          least number of squares (A002828) that add up to k}.
 * https://oeis.org/A260732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 434, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A260732:
   a(n) = number of steps needed to reach zero starting from
   k = n^2 and repeatedly subtracting the minimal number of
   squares that sum to k.
   The implementation precomputes square and 2‑square
   representations up to the required value, so each step
   runs in O(1) time. */
#include <stdlib.h>
#include <stdint.h>

/* Global caches for the representation tables.  They are
   rebuilt when a larger n is requested. */
static uint8_t *is_two = NULL;      /* is_two[k] = 1 if k is sum of two squares */
static uint8_t *is_sq  = NULL;      /* is_sq[k]  = 1 if k is a perfect square */
static size_t  cached_max_k = 0;    /* largest k for which tables are built */

/* Rebuild the tables to cover all k <= n^2.  This is called
   lazily by the main routine. */
static void init_cache(idx_t n)
{
    size_t N = (size_t)n;
    size_t max_k = N * N;
    if (max_k <= cached_max_k)
        return;                       /* already built */

    /* Free previous tables */
    free(is_two);
    free(is_sq);

    /* Allocate new tables and zero‑initialise them */
    is_two = (uint8_t*)calloc(max_k + 1, sizeof(uint8_t));
    is_sq  = (uint8_t*)calloc(max_k + 1, sizeof(uint8_t));
    if (!is_two || !is_sq)
        exit(1);                      /* memory allocation failed */

    /* Mark perfect squares */
    for (size_t i = 0; i * i <= max_k; ++i)
        is_sq[i * i] = 1;

    /* Mark sums of two squares: a^2 + b^2 with a <= b */
    for (size_t a = 0; a * a <= max_k; ++a) {
        size_t a2 = a * a;
        for (size_t b = a; a2 + b * b <= max_k; ++b)
            is_two[a2 + b * b] = 1;
    }

    cached_max_k = max_k;
}

/* Return the minimal number of squares that represent k.  The
   function assumes that init_cache has been called for a limit
   at least k. */
static unsigned char min_squares_single(seq_t k)
{
    if (k == 0)
        return 0;

    /* Perfect square test */
    if (is_sq[(size_t)k])
        return 1;

    /* Sum of two squares test */
    if (is_two[(size_t)k])
        return 2;

    /* Legendre's 4‑square theorem: if k = 4^a(8b + 7) then 4 squares
       are required; otherwise 3 squares suffice. */
    seq_t t = k;
    while ((t % (seq_t)4) == 0)
        t /= 4;
    if ((t % (seq_t)8) == 7)
        return 4;

    return 3;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    init_cache(n);

    seq_t k = (seq_t)n * (seq_t)n;
    seq_t steps = 0;

    while (k > 0) {
        unsigned char g = min_squares_single(k);
        k -= (seq_t)g;
        ++steps;
    }
    return steps;
}
