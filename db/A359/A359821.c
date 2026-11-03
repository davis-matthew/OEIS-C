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
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate integers and select those whose arithmetic derivative
   (OEIS A003415) has parity opposite to the integer itself.  The function
   builds the list on demand and caches it for fast repeated queries. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --------------------------------------------------------------------
   Return true iff k has the property that its arithmetic derivative
   has parity opposite to k.  The derivative of k = Π p^e is
   D(k) = Σ e * (k/p), so its parity is the XOR of the terms
   (e%2) AND ((k/p)%2) for each distinct prime factor p.
   -------------------------------------------------------------------- */
static bool has_opposite_parity(uint64_t k)
{
    uint64_t original = k;
    bool deriv_parity = false; /* D(k) mod 2, accumulated XOR */

    /* Factorize k by trial division.  The candidate value k fits in
       64‑bit, which is sufficient for the test data (n up to ~10⁴).   */
    for (uint64_t p = 2; p * p <= k; ++p) {
        if (k % p == 0) {
            uint64_t exp = 0;
            while (k % p == 0) { k /= p; ++exp; }
            if ((exp & 1ULL) && ((original / p) & 1ULL))
                deriv_parity = !deriv_parity;
        }
    }
    /* Remaining prime factor, if any. */
    if (k > 1) {
        uint64_t exp = 1;
        if ((exp & 1ULL) && ((original / k) & 1ULL))
            deriv_parity = !deriv_parity;
    }

    /* Return true if parity of D(k) differs from parity of k. */
    return (deriv_parity != ((original & 1ULL) != 0));
}

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
        seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
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
