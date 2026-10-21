/*
 * A268278  Numbers n such that both n and n^2 contain "123" as a substring.
 * https://oeis.org/A268278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   contains_123_generic, contains_123_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation of A268278: numbers n such that
   decimal representation of n and n^2 both contain "123". */

/* We use only standard integer arithmetic.  For 64‑bit builds we employ
   unsigned __int128 to avoid overflow when squaring.  For larger
   arbitrary‑precision seq_t types the multiplication is handled by the
   built‑in operator. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/contains_123_generic.h"
#include "../utils/contains_123_uint128.h"

/* For 128‑bit types we reimplement the same routine to avoid overflow
   when casting to 64‑bit. */
#ifdef __SIZEOF_INT128__

#endif

/* Static sequence cache: the found terms and the next integer to test. */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;   /* number of elements currently stored   */
static size_t seq_cap  = 0;   /* allocated capacity                    */
static seq_t candidate = 1;   /* next integer to test (positive)       */

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defensive, not part of the sequence */

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        seq_t x = candidate++;

        /* Only numbers that explicitly contain 123 are worth testing. */
        if (!contains_123_generic((unsigned long long)x))
            continue;

        /* Compute x^2 without overflow when seq_t is 64‑bit. */
#ifdef __SIZEOF_INT128__
        unsigned __int128 sq128 = (unsigned __int128)x * (unsigned __int128)x;
        if (!contains_123_uint128(sq128))
            continue;
#else
        seq_t sq = x * x;
        if (!contains_123_generic((unsigned long long)sq))
            continue;
#endif

        /* Store x in the sequence cache. */
        if (seq_len + 1 > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 64;
            if (new_cap < seq_len + 1) new_cap = seq_len + 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure – unlikely */
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len++] = x;
    }

    return seq[n - 1];
}
