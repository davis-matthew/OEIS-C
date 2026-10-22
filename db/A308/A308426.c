/*
 * A308426  Sum of the smallest side lengths of all integer-sided triangles
 *          with perimeter n whose sides are nonsquarefree.
 * https://oeis.org/A308426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the smallest side lengths of all integer‑sided *
 *            triangles with perimeter n whose three sides are non‑squarefree.
 *            * The algorithm enumerates admissible triples (a,b,c) with a ≤ b
 *            ≤ c, * a + b > c, a + b + c = n, and checks the nonsquarefree
 *            property * of each side by marking multipl...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of nonsquarefree flags: 1 = nonsquarefree, 0 = squarefree */
static uint8_t *nonsq_cache = NULL;
static idx_t nonsq_cache_size = 0;

/* Ensure the nonsquarefree cache has been computed up to at least N (inclusive). */
static void ensure_nonsq_cache(idx_t N)
{
    if (N + 1 <= nonsq_cache_size)
        return;                            /* already computed up to N */

    /* Grow the array to hold indices 0..N */
    size_t new_size = (size_t)N + 1;
    uint8_t *tmp = (uint8_t *)realloc(nonsq_cache, new_size);
    if (!tmp)
        exit(1);                            /* allocation failed */

    /* Initialize newly added entries to 0 (squarefree) */
    size_t old_size = (size_t)nonsq_cache_size;
    for (size_t i = old_size; i < new_size; ++i)
        tmp[i] = 0;

    nonsq_cache = tmp;
    nonsq_cache_size = new_size;

    /* Mark all numbers having a square divisor > 1.
     * For each square s = p^2, set multiples of s to 1. */
    for (idx_t p = 2; p * p <= N; ++p) {
        idx_t sq = p * p;
        for (idx_t j = sq; j <= N; j += sq)
            nonsq_cache[j] = 1;              /* nonsquarefree */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_nonsq_cache(n);

    seq_t sum = 0;

    for (idx_t a_side = 1; a_side <= n / 3; ++a_side) {
        if (!nonsq_cache[(size_t)a_side])
            continue;                       /* a must be nonsquarefree */

        idx_t max_b = (n - a_side) / 2;
        for (idx_t b_side = a_side; b_side <= max_b; ++b_side) {
            if (!nonsq_cache[(size_t)b_side])
                continue;                   /* b must be nonsquarefree */

            idx_t c_side = n - a_side - b_side;
            if (!nonsq_cache[(size_t)c_side])
                continue;                   /* c must be nonsquarefree */

            if (a_side + b_side <= c_side)
                continue;                   /* triangle inequality failed */

            sum += (seq_t)a_side;            /* add the smallest side */
        }
    }

    return sum;
}
