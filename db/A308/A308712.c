/*
 * A308712  a(0) = 0 and a(1) = 1; for n > 1, a(n) = a(n-1)/2 if that number
 *          is an integer and not already in the sequence, otherwise a(n) =
 *          3*a(n-1) + remainder of a(n-1)/2. (A variant of the Collatz
 *          sequence).
 * https://oeis.org/A308712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1079, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0
   a(1) = 1
   for n > 1:
     if a(n-1)/2 is an integer and not already in the sequence
        a(n) = a(n-1)/2;
     otherwise
        a(n) = 3*a(n-1) + (a(n-1) mod 2)   (remainder of dividing by 2) */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static storage for memoisation */
static seq_t *seq_vals   = NULL;   /* array of computed a(k) values   */
static size_t seq_cap   = 0;       /* current capacity of seq_vals     */
static idx_t   seq_len   = 0;       /* number of values stored (>=2)    */

/* Ensure the array has room for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (seq_cap == 0) {
        seq_cap = 16;
        seq_vals = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }
    if ((size_t)n + 1 > seq_cap) {
        while ((size_t)n + 1 > seq_cap) seq_cap <<= 1;
        seq_vals = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
    }
}

/* Linear search for membership */
static bool contains(seq_t v)
{
    for (idx_t i = 0; i < seq_len; ++i)
        if (seq_vals[i] == v) return true;
    return false;
}

/* Main sequence function required by the OEIS entry */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Initialise on first call */
    if (seq_vals == NULL) {
        ensure_capacity(1);
        seq_vals[0] = (seq_t)0;
        seq_vals[1] = (seq_t)1;
        seq_len = 2;
    }

    /* Compute terms up to the requested index */
    for (; seq_len <= n; ++seq_len) {
        seq_t prev = seq_vals[seq_len - 1];
        seq_t next;

        if ((prev % (seq_t)2) == 0) {          /* even */
            seq_t cand = prev / (seq_t)2;
            if (!contains(cand)) next = cand;
            else                   next = prev * (seq_t)3; /* remainder 0 */
        } else {                               /* odd */
            next = prev * (seq_t)3 + (seq_t)1; /* remainder 1 */
        }

        ensure_capacity(seq_len);
        seq_vals[seq_len] = next;
    }

    return seq_vals[n];
}
