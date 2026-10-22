/*
 * A360476  The integers of the sequence appear exactly twice. Between the two
 *          copies of k there are k odd integers. The sequence is always
 *          extended with the smallest integer not leading to a contradiction.
 * https://oeis.org/A360476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A360476: sequence defined by “each integer k occurs exactly twice, and
   between the two copies of k there are exactly k odd integers”.
   The sequence is built by always appending the smallest integer that
   does not lead to a contradiction.  This implementation maintains
   pending occurrences, remaining odd counts, and the implied zero‑count
   constraints, and extends the sequence lazily up to the requested index. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* static storage for the built sequence */
static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;          /* allocated capacity of seq_vals   */
static idx_t seq_len = 0;           /* number of terms currently known */

/* arrays recording first and second positions for every value seen */
static idx_t *pos1 = NULL;
static idx_t *pos2 = NULL;
static idx_t *rem  = NULL;          /* remaining odds needed before second copy */
static size_t val_cap = 0;          /* capacity of the above arrays */

/* list of values that have appeared once but not yet twice */
static idx_t *pending_vals = NULL;  /* values pending completion */
static size_t pending_len = 0;      /* length of pending_vals          */
static size_t pending_cap = 0;      /* allocated capacity */

/* number of pending values whose remaining required odds have reached 0 */
static size_t zero_pending_count = 0;

/* utility: ensure capacity of sequence array */
static void ensure_seqcap(idx_t need)
{
    if (need <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < (size_t)need) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_vals, newcap * sizeof(seq_t));
    seq_vals = tmp;
    for (size_t i = seq_cap; i < newcap; ++i) seq_vals[i] = (seq_t)0;
    seq_cap = newcap;
}

/* utility: ensure capacity of value‑based arrays */
static void ensure_valcap(idx_t need)
{
    if (need <= val_cap) return;
    size_t newcap = val_cap ? val_cap * 2 : 16;
    while (newcap < (size_t)need) newcap *= 2;
    idx_t *tmp1 = (idx_t *)realloc(pos1, newcap * sizeof(idx_t));
    idx_t *tmp2 = (idx_t *)realloc(pos2, newcap * sizeof(idx_t));
    idx_t *tmp3 = (idx_t *)realloc(rem , newcap * sizeof(idx_t));
    pos1 = tmp1; pos2 = tmp2; rem = tmp3;
    for (size_t i = val_cap; i < newcap; ++i) {
        pos1[i] = 0; pos2[i] = 0; rem[i] = 0;
    }
    val_cap = newcap;
}

/* utility: ensure capacity of pending list */
static void ensure_pendingcap(size_t need)
{
    if (need <= pending_cap) return;
    size_t newcap = pending_cap ? pending_cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    idx_t *tmp = (idx_t *)realloc(pending_vals, newcap * sizeof(idx_t));
    pending_vals = tmp;
    pending_cap = newcap;
}

/* locate a value in the pending list; returns index or pending_len if not found */
static size_t find_pending(idx_t v)
{
    for (size_t i = 0; i < pending_len; ++i)
        if (pending_vals[i] == v) return i;
    return pending_len;
}

/* a(idx_t n) implements the sequence */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset starts at 1 */

    /* extend the sequence up to n */
    while (seq_len < n) {
        idx_t position = seq_len + 1;      /* 1‑based position of next term */
        idx_t candidate = 1;

        /* search smallest admissible candidate */
        for (;;) {
            ensure_valcap(candidate + 1);  /* allow access to pos1/pos2/rem */

            /* already twice? */
            if (pos2[candidate] != 0) { candidate++; continue; }

            bool allowed = false;

            /* candidate occurs first time */
            if (pos1[candidate] == 0) {
                /* odd first occurrence must not be prohibited by pending zeros */
                if ((candidate & 1) == 0)                 allowed = true; /* even */
                else if (zero_pending_count == 0)          allowed = true; /* odd, no pending zero */
                else allowed = false;                       /* odd would create extra odd */
            }
            /* candidate is a pending number */
            else {
                if (rem[candidate] == 0) {              /* second occurrence possible */
                    if (zero_pending_count == 1)            allowed = true; /* only this one zero */
                    else if (zero_pending_count == 0)         allowed = true; /* shouldn't happen */
                    else                                    /* multiple zeros present */
                        allowed = ((candidate & 1) == 0); /* even can proceed */
                } else
                    allowed = false; /* still needs odd counts */
            }

            if (!allowed) { candidate++; continue; }

            /* ---- candidate accepted ---- */
            ensure_seqcap(seq_len + 2);          /* safety for new element */
            seq_vals[seq_len++] = (seq_t)candidate;

            bool is_odd = (candidate & 1) != 0;

            /* handle first occurrence */
            if (pos1[candidate] == 0) {
                /* decrement all pending odds before adding the new pending */
                if (is_odd) {
                    for (size_t i = 0; i < pending_len; ++i) {
                        idx_t v = pending_vals[i];
                        if (rem[v] > 0) {
                            rem[v]--;
                            if (rem[v] == 0) zero_pending_count++;
                        }
                    }
                }
                pos1[candidate] = position;
                rem[candidate]  = (idx_t)candidate;               /* require this many odds */
                ensure_pendingcap(pending_len + 1);
                pending_vals[pending_len++] = candidate;            /* add to pending */
                /* rem > 0, zero_pending_count unchanged */
            }
            /* handle second occurrence */
            else {
                pos2[candidate] = position;
                /* remove from pending list */
                size_t idx = find_pending(candidate);
                if (idx < pending_len) {
                    if (idx < pending_len - 1) pending_vals[idx] = pending_vals[pending_len - 1];
                    pending_len--;
                }
                /* reduce zero_pending_count if this was a zero pending */
                if (rem[candidate] == 0) zero_pending_count--;
                rem[candidate] = 0;
                /* decrement remaining odds of the remaining pending numbers if odd */
                if (is_odd) {
                    for (size_t i = 0; i < pending_len; ++i) {
                        idx_t v = pending_vals[i];
                        if (rem[v] > 0) {
                            rem[v]--;
                            if (rem[v] == 0) zero_pending_count++;
                        }
                    }
                }
            }

            break; /* move to next position */
        }
    }

    return seq_vals[n - 1];
}
