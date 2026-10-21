/*
 * A253260  Brazilian squares.
 * https://oeis.org/A253260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth square that is a Brazilian number (i.e. can be
 *            written * as a repdigit in some base 2 ≤ b ≤ a−2 with digit d, 1
 *            ≤ d < b). * The term for index n ≥ 1 is obtained by generating
 *            squares * and checking each for the Brazilian property. See
 *            function * is_brazilian_square for the test logic
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 405, u64 n <= 405,
 *            u128 n <= 405, big any n
 * Helpers:   is_brazilian_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_brazilian_square.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Cache to store previously generated terms.                          */
/* ------------------------------------------------------------------ */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static idx_t  terms    = 0;          /* number of terms stored (≥ 0) */
static idx_t  next_m   = 2;          /* next integer to try (m^2) */

/* Ensure the cache has capacity at least 'needed'. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);  /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public function that returns the nth Brazilian square (n ≥ 1).     */
/* The function memoizes results, so calls in any order are fast.    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for 0, return 0 */

    /* Generate terms until we reach the requested index. */
    while (terms < n) {
        seq_t m = (seq_t)next_m;
        seq_t square = m * m;
        if (is_brazilian_square(square)) {
            ensure_cache_capacity(terms + 1);
            cache[terms++] = square;
        }
        ++next_m;
    }
    return cache[n - 1];
}
