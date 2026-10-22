/*
 * A342441  a(1) = 1; for n > 1, a(n) is the least positive integer not
 *          occurring earlier such that a(n-1)+a(n) shares no digit with
 *          either a(n-1) or a(n).
 * https://oeis.org/A342441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1.
   For n > 1, a(n) is the smallest positive integer not yet appearing in
   the sequence such that (a(n‑1) + a(n)) shares no decimal digit with
   a(n‑1) or with a(n). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *seq      = NULL;   /* array of computed terms   */
static size_t seq_cap  = 0;      /* allocated capacity of seq */
static idx_t  seq_len  = 0;      /* number of terms stored    */

static uint8_t *used   = NULL;   /* bitmap of used integers  */
static size_t   used_cap = 0;    /* allocated capacity of used */

/* Ensure the bitmap can contain indices up to 'max_val'.  The bitmap
   is stored as an array of bytes where each bit corresponds to a
   candidate value. */
static void ensure_used_capacity(idx_t max_val)
{
    if ((size_t)max_val < used_cap / 8)
        return;
    size_t new_bytes = used_cap ? used_cap * 2 : 64;
    while ((size_t)max_val >= new_bytes * 8)
        new_bytes *= 2;
    used = (uint8_t*)realloc(used, new_bytes);
    if (!used) exit(EXIT_FAILURE);
    memset(used + used_cap, 0, new_bytes - used_cap);
    used_cap = new_bytes;
}

/* Mark a value as used.  The value must be ≤ (used_cap*8)-1. */
static void mark_used(idx_t val)
{
    ensure_used_capacity(val);
    used[val >> 3] |= (1u << (val & 7));
}

/* Test whether a value has already been used. */
static bool is_used(idx_t val)
{
    if ((size_t)val >= used_cap * 8)
        return false;
    return (used[val >> 3] & (1u << (val & 7))) != 0;
}

/* Ensure the seq array can hold at least 'min_len' elements. */
static void ensure_seq_capacity(idx_t min_len)
{
    if ((size_t)min_len <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while ((size_t)min_len > new_cap)
        new_cap *= 2;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* Return true iff two decimal numbers share a common digit. */
static bool has_common_digit(seq_t a, seq_t b)
{
    bool seen[10] = {0};

    if (a == (seq_t)0) seen[0] = true;
    seq_t ta = a;
    while (ta != (seq_t)0) {
        unsigned int d = (unsigned)(ta % (seq_t)10);
        seen[d] = true;
        ta /= (seq_t)10;
    }

    if (b == (seq_t)0)
        return seen[0];
    seq_t tb = b;
    while (tb != (seq_t)0) {
        unsigned int d = (unsigned)(tb % (seq_t)10);
        if (seen[d]) return true;
        tb /= (seq_t)10;
    }
    return false;
}

/* Compute the n-th term of the sequence.  The function is memoised
   so that calls with non‑monotonic n are served efficiently. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n=0; safe default */

    if (seq == NULL)                /* initialise storage once */
        ensure_seq_capacity(64);
    if (used == NULL)
        ensure_used_capacity(64);

    while (seq_len < n) {
        if (seq_len == (idx_t)0) {   /* a(1) = 1 */
            seq[0] = (seq_t)1;
            mark_used(1);
            seq_len = 1;
            continue;
        }

        seq_t prev = seq[seq_len - 1];
        seq_t candidate = (seq_t)1;

        while (true) {
            idx_t cand_idx = (idx_t)candidate;
            if (!is_used(cand_idx)) {
                seq_t sum = prev + candidate;
                if (!has_common_digit(sum, prev) &&
                    !has_common_digit(sum, candidate))
                    break;          /* candidate is valid */
            }
            ++candidate;
        }

        ensure_seq_capacity(seq_len + 1);
        seq[seq_len] = candidate;
        mark_used((idx_t)candidate);
        ++seq_len;
    }

    return seq[n - 1];
}
