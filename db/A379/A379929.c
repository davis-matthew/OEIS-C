/*
 * A379929  Numbers that have the same number of prime factors, counted with
 *          multiplicity, as there are runs in their base-10 representation.
 * https://oeis.org/A379929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 9894, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8932 (2026-11-03)
 * Helpers:   bigomega_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379929 – numbers k with Ω(k) = runs in decimal representation.
   The implementation searches candidates incrementally, storing
   results in a cache so that subsequent calls are O(1) after the
   required terms have been found. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega_seq.h"
#include "../utils/mem.h"

/* Count the number of runs (maximal consecutive blocks of equal
   decimal digits) in a positive seq_t number. */
static unsigned int runs_in_decimal(seq_t x)
{
    seq_t temp = x;
    unsigned int runs = 0;
    unsigned int last_digit = 10;        /* sentinel value */
    while (temp > (seq_t)0) {
        unsigned int digit = (unsigned int)(temp % (seq_t)10);
        if (digit != last_digit) {
            runs++;
            last_digit = digit;
        }
        temp /= (seq_t)10;
    }
    return runs;
}

/* Determine whether a candidate k satisfies Ω(k) == runs(k). */
static bool matches_idx(idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t omega = bigomega_seq(kk);
    unsigned int runs = runs_in_decimal(kk);
    return omega == (seq_t)runs;
}

/* Cached sequence storage. */
static seq_t *seq_vals = NULL;     /* array holding found terms            */
static size_t seq_cap  = 0;        /* current allocated capacity           */
static size_t seq_len  = 0;        /* number of terms currently stored     */
static idx_t  seq_last_found = 0;  /* last number examined in the search   */
static bool   seq_initialized = false;

/* Ensure 'seq_vals' has at least 'need' entries.  Dynamic array is
   doubled each time it needs to grow. */
static void grow_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_vals = mem_resize(seq_t, seq_vals, new_cap);
    seq_cap  = new_cap;
}

/* Public function: returns the n‑th term of A379929 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1 */

    if (!seq_initialized) {                     /* lazy initialization */
        seq_initialized = true;
        seq_cap   = 16;
        seq_vals  = mem_new(seq_t, seq_cap);
        seq_len   = 0;
        seq_last_found = 0;
    }

    /* Return cached value if available. */
    if ((size_t)n <= seq_len) {
        return seq_vals[n - 1];
    }

    /* Extend the cache until at least n terms have been found. */
    while (seq_len < (size_t)n) {
        seq_last_found++;                         /* next candidate */
        if (matches_idx(seq_last_found)) {
            grow_seq_capacity(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)seq_last_found;
        }
    }

    return seq_vals[n - 1];
}
