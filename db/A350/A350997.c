/*
 * A350997  a(n) = Sum_{k=1..n} k^rad(k).
 * https://oeis.org/A350997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^rad(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-03)
 * Helpers:   ipow, mem, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"
#include "../utils/radical.h"
#include "../utils/mem.h"

/* Stateful cache of cumulative sums: cumulative[0] = 0, cumulative[n] = a(n) */
static seq_t *cumulative = NULL;          /* array of seq_t values */
static size_t cumulative_cap = 0;         /* allocated capacity (number of elements) */
static idx_t   cumulative_len = 0;        /* largest n for which cumulative[n] is valid */

/* Ensure the cache can hold indices up to new_cap – 1 */
static void ensure_cumulative_size(size_t new_cap)
{
    if (new_cap > cumulative_cap) {
        seq_t *tmp = mem_resize(seq_t, cumulative, new_cap);
        if (!tmp) return;               /* allocation failed – leave cache unchanged */
        cumulative = tmp;
        cumulative_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset is 1; gracefully handle 0 */

    size_t need_cap = (size_t)n + 1;      /* need indices 0..n inclusive */
    ensure_cumulative_size(need_cap);
    if (!cumulative) return (seq_t)0;     /* allocation failed */

    /* Initialise base case if cache empty */
    if (cumulative_len == (idx_t)0 && cumulative_cap > 0) {
        cumulative[0] = (seq_t)0;
        cumulative_len = 0;
    }

    /* Compute missing terms up to n */
    for (size_t k = (size_t)(cumulative_len + 1); k <= (size_t)n; ++k) {
        idx_t     ik   = (idx_t)k;
        seq_t     radk = radical(ik);                 /* radical of k */
        seq_t     pk   = ipow((seq_t)ik, radk);/* k^rad(k) */
        cumulative[k] = cumulative[k - 1] + pk;
    }

    cumulative_len = n;
    return cumulative[(size_t)n];
}
