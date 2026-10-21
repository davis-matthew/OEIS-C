/*
 * A242018  Starting with a(0) = 0, a(1) = 0, a(2) = 1, repeatedly append to
 *          this sequence the last half of the currently-existing terms,
 *          taking the longer half if there are an odd number of terms.
 * https://oeis.org/A242018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=0, a(1)=0, a(2)=1;
   For k≥3, let L be the current length of the built prefix.
   Let m = ceil(L/2).  Append the last m elements of the prefix.
   This defines all terms a(n).  */
#include <stddef.h>                     /* size_t */
#include "../utils/seq_alloc.h"          /* seq_alloc */
#include "../utils/seq_realloc.h"       /* seq_realloc */

/* Static memoisation storage ------------------------------------------------- */
static seq_t *seq      = NULL;   /* array of computed terms              */
static idx_t  seq_len  = 0;      /* number of terms already stored       */
static idx_t  seq_cap  = 0;      /* capacity of the array (elements)     */

/* Ensure the array has capacity at least 'need' elements.  */
static void grow_cap(idx_t need)
{
    if (seq_cap >= need) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
    /* In case reallocation fails, leave the old array untouched.  */
    if (tmp != NULL) {
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* Return a(n).  -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* --- initialise if this is the first call --------------------------------- */
    if (seq_len == 0) {
        seq_cap = 3;
        seq     = seq_alloc(seq_cap);
        if (seq == NULL) return 0;          /* out of memory – return 0      */
        seq[0] = 0;
        seq[1] = 0;
        seq[2] = 1;
        seq_len = 3;
    }

    /* --- already computed? ----------------------------------------------------- */
    if (n < seq_len) return seq[n];

    /* --- grow the sequence step by step until it contains index n --------- */
    while (seq_len <= n) {
        idx_t m        = (seq_len + 1) / 2;      /* ceil(seq_len/2)         */
        idx_t new_len  = seq_len + m;
        grow_cap(new_len);

        /* copy the last 'm' elements to the new slots */
        for (idx_t i = 0; i < m; ++i) {
            seq[seq_len + i] = seq[seq_len - m + i];
        }
        seq_len = new_len;
    }
    return seq[n];
}
