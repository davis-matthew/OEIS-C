/*
 * A359821  Numbers k whose arithmetic derivative, A003415(k), has the
 *          opposite parity to k.
 * https://oeis.org/A359821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   has_opposite_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate integers and select those whose arithmetic derivative
   (OEIS A003415) has parity opposite to the integer itself.  The function
   builds the list on demand and caches it for fast repeated queries. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_opposite_parity.h"

/* --------------------------------------------------------------------
   Cached sequence values and bookkeeping.
   -------------------------------------------------------------------- */
static seq_t *seq_cache = NULL;   /* dynamic array of found terms   */
static size_t  seq_len  = 0;      /* number of terms currently cached */
static size_t  seq_cap  = 0;      /* capacity of seq_cache             */
static uint64_t last_checked = 0; /* largest integer already tested    */

/* --------------------------------------------------------------------
   Append a new term to the cached sequence, expanding storage as needed.
   -------------------------------------------------------------------- */
static void add_term(uint64_t value)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!seq_cache) abort();
    }
    seq_cache[seq_len++] = (seq_t)value;
}

/* --------------------------------------------------------------------
   Ensure that the cached sequence contains at least n terms.
   -------------------------------------------------------------------- */
static void ensure_upto(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    while (seq_len < (size_t)n) {
        ++last_checked;
        if (has_opposite_parity(last_checked))
            add_term(last_checked);
    }
}

/* --------------------------------------------------------------------
   Public sequence function.  a(1) is the first term listed in the
   problem statement.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, guard */

    ensure_upto(n);
    return seq_cache[(size_t)(n - 1)];
}
