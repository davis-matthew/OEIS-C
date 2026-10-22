/*
 * A396057  a(1)=1; if a(n-1) is odd then a(n) = a(n-1)*2+2; if a(n-1) is even
 *          then a(n) = a(n-1)/2 if not already in the sequence, otherwise
 *          a(n) = a(n-1)+1.
 * https://oeis.org/A396057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition of OEIS A396057
   a(1) = 1
   a(n) = a(n-1)*2+2,                 if a(n-1) is odd
   a(n) = a(n-1)/2,   if a(n-1) is even and a(n-1)/2 not yet in the sequence
   a(n) = a(n-1)+1,  otherwise
   The function memoises the sequence up to the largest n requested.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *seq = NULL;   /* array of computed terms */
static size_t seq_len = 0;  /* number of terms stored */
static size_t seq_cap = 0;  /* capacity of seq array */

/* Ensure the dynamic array has enough capacity for at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) abort();            /* out of memory */
    seq = new_seq;
    seq_cap = new_cap;
}

/* Computes the n-th element of the sequence (a(1) is term 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return 0 */
    /* Initialise the sequence with the first term if needed. */
    if (seq_len == 0) {
        ensure_capacity(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
    }
    /* Compute terms up to the requested index. */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        seq_t next;
        if (prev & (seq_t)1) {                     /* prev is odd */
            next = prev * ((seq_t)2) + ((seq_t)2);
        } else {                                  /* prev is even */
            seq_t cand = prev / ((seq_t)2);
            bool found = false;
            /* Linear search: the sequence is small (n <= ~1e4). */
            for (size_t i = 0; i < seq_len; ++i) {
                if (seq[i] == cand) { found = true; break; }
            }
            if (!found) next = cand;
            else        next = prev + ((seq_t)1);
        }
        ensure_capacity(seq_len + 1);
        seq[seq_len++] = next;
    }
    return seq[n - 1];
}
