/*
 * A347268  a(n) is the first term of the n-th 3x+1 sequence that shares
 *          infinitely many 1's with the 3x+1 sequence that starts at 2.
 * https://oeis.org/A347268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th natural number m whose Collatz total
 * stopping time is congruent to 1 (mod 3).  The sequence is generated
 * by scanning integers upwards and selecting those with that property. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Computes (total stopping time of m) % 3.  The value is 0 for m==1,
 * 1 when the number of steps to reach 1 leaves remainder 1, etc. */
static unsigned total_stop_mod3(seq_t m)
{
    unsigned mod = 0;            /* current total stopping time modulo 3 */
    seq_t x = m;
    while (x != (seq_t)1) {
        if ((x & (seq_t)1) == 0)          /* even */
            x = x >> 1;
        else                              /* odd */
            x = x * ((seq_t)3) + (seq_t)1;
        mod += 1;
        if (mod >= 3) mod -= 3;           /* keep mod in {0,1,2} */
    }
    return mod;
}

/* ----------------- helper: total stopping time modulo 3 ----------------- */

/* ----------------- dynamic cache of valid terms ----------------- */
static seq_t *cache = NULL;      /* array of terms */
static idx_t  cache_len = 0;     /* number of terms stored   */
static idx_t  cache_cap = 0;     /* allocated capacity       */
static seq_t  last_checked = (seq_t)1;  /* last m examined       */

/* Ensure that the cache can hold at least 'needed' terms.  Caller
 * guarantees that 'needed' > 'cache_cap' if a reallocation is
 * necessary. */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_arr = mem_resize(seq_t, cache, (size_t)new_cap);
    if (!new_arr)                    /* allocation failure – leave cache */
        return;
    cache = new_arr;
    cache_cap = new_cap;
}

/* ----------------- public sequence function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for n==0 */

    /* Generate terms until we have at least n cached. */
    while (cache_len < n) {
        seq_t m = ++last_checked;           /* next candidate number */

        if (total_stop_mod3(m) == 1) {       /* condition for the sequence */
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = m;
        }
    }
    return cache[n - 1];
}
