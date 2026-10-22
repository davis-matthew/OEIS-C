/*
 * A347809  Expansion of (theta_3(x) - 1)^6 / (32 * (3 - theta_3(x))).
 * https://oeis.org/A347809
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A347809 – number of compositions of n into 6 or more squares.
 *  a(n) = number of ordered partitions of n into squares, with at least
 *  six parts.  This is computed by dynamic programming:
 *
 *  dp_total[i]  – number of compositions of i using any number of parts.
 *  dp_k[k][i]   – number of compositions of i using exactly k parts
 *                 (k = 1..5).  The answer is dp_total[n] minus the
 *                 sum of dp_k[1]..dp_k[5] for that n.
 *
 *  The tables are lazily expanded when a larger n is requested.
 *  The algorithm runs in O(n * sqrt(n)) for the first call to a new
 *  maximum n, which is easily fast enough for n up to 10‑000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build.
   No assumptions on their exact type except those in the
   problem statement.  All integer arithmetic uses seq_t or idx_t
   as appropriate.                                 */

static seq_t *dp_total = NULL;          /* dp_total[i]  */
static seq_t *dp_k[6] = {NULL};        /* dp_k[1..5][i]  */
static idx_t  max_size = 0;            /* current table size */


/* Integer square root: floor(sqrt(n)).  Simple integer loop is fine
   for n <= 10⁴, which is the practical limit in this context.   */
static idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}

/* Ensure the tables are large enough for indices up to max_needed.
   Process in three stages: allocate space, copy old data, compute
   new entries.  All allocations use realloc for minimal overhead. */
static void ensure_capacity(idx_t max_needed)
{
    if (max_needed <= max_size) return;   /* already sufficient */

    /* --- Allocate new space ------------------------------------------------ */
    /* dp_total */
    seq_t *new_total = (seq_t*)realloc(dp_total,
                                       (max_needed + 1) * sizeof(seq_t));
    if (new_total == NULL) abort();   /* Out of memory */
    dp_total = new_total;

    /* dp_k[1..5] */
    for (int k = 1; k <= 5; ++k) {
        seq_t *new_k = (seq_t*)realloc(dp_k[k],
                                       (max_needed + 1) * sizeof(seq_t));
        if (new_k == NULL) abort();
        dp_k[k] = new_k;
    }

    /* --- Initialise new entries -------------------------------------------- */
    /* dp_total[0] = 1, dp_k[1..5][0] = 0 (already zero for new memory) */
    if (max_size == 0) dp_total[0] = (seq_t)1;

    /* Compute dp_total for new indices */
    for (idx_t i = max_size + 1; i <= max_needed; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j * j <= i; ++j) {
            sum += dp_total[i - j * j];
        }
        dp_total[i] = sum;
    }

    /* dp_k[1] – single square compositions */
    for (idx_t i = max_size + 1; i <= max_needed; ++i) {
        seq_t val = (seq_t)0;
        idx_t r = isqrt(i);
        if (r * r == i) val = (seq_t)1;      /* perfect square */
        dp_k[1][i] = val;
    }

    /* dp_k[2..5] – compositions with more parts */
    for (int k = 2; k <= 5; ++k) {
        for (idx_t i = max_size + 1; i <= max_needed; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j * j <= i; ++j) {
                sum += dp_k[k - 1][i - j * j];
            }
            dp_k[k][i] = sum;
        }
    }

    max_size = max_needed;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The OEIS offset is 6; for n < 6 the value is unspecified. */
    if (n < 6) return (seq_t)0;

    ensure_capacity(n);

    seq_t result = dp_total[n];
    for (int k = 1; k <= 5; ++k)
        result -= dp_k[k][n];
    return result;
}
