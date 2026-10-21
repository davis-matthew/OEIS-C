/*
 * A257262  Numbers such that the least missing nonzero digit (A257079) in
 *          their factorial base representation is 2.
 * https://oeis.org/A257262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 9361, u32 n <= 9361, u64 n <= 9361,
 *            u128 n <= 9361, big any n
 * Helpers:   good_factorial_rep
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257262: numbers whose factorial base representation has
   the least missing non‑zero digit equal to 2.
   The sequence consists of integers whose factorial base (A007623)
   contains at least one digit 1 and no digit 2. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_factorial_rep.h"

/* Ensure the dynamic array has space for at least 'need' elements. */
static void ensure_capacity(size_t need, size_t *cap, seq_t **array)
{
    if (need <= *cap)
        return;
    size_t newcap = (*cap) ? (*cap << 1) : 8;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*array, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* allocation failure */
    *array = tmp;
    *cap   = newcap;
}

/* Main sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                         /* offset is 1 */
        return (seq_t)0;

    /* Static state retained between calls */
    static seq_t *cache   = NULL;     /* cached terms (0‑based) */
    static size_t cap     = 0;         /* cache capacity         */
    static size_t cnt     = 0;         /* number of cached terms */
    static idx_t  next    = 1;         /* next integer to test   */

    while (cnt < n)
    {
        seq_t val = (seq_t)next;
        if (good_factorial_rep(val))
        {
            ensure_capacity(cnt + 1, &cap, &cache);
            cache[cnt++] = val;
        }
        ++next;
    }
    return cache[n - 1];                /* a(1) corresponds to cache[0] */
}
