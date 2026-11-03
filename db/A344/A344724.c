/*
 * A344724  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^n.
 * https://oeis.org/A344724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 360 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for n=0, not used normally */

    size_t sz = (size_t)n + 1;           /* enough slots for bases 1..n */
    seq_t *pow_cache = alloc_seq_array(sz);
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t base = n / k;              /* floor(n/k) */
        seq_t term = pow_cache[(size_t)base];
        if (term == 0) {                 /* not yet computed */
            term = ipow_idx((seq_t)base, n);
            pow_cache[(size_t)base] = term;
        }
        if (k & 1)                       /* odd k -> add */
            sum += term;
        else                             /* even k -> subtract */
            sum -= term;
    }

    free_seq_array(pow_cache);
    return sum;
}
