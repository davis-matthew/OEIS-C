/*
 * A304502  Solution (c(n)) of the system of complementary equations defined
 *          in Comments.
 * https://oeis.org/A304502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, b(0) = 2, c(0) = a(0)+2*b(0) = 5.
   For n > 0:
       a(n) = least positive integer not appearing among any of
              {a(k), b(k), c(k)} for k < n;
       b(n) = least positive integer not appearing among the same set
              for k < n and after a(n) has been selected;
       c(n) = a(n) + 2*b(n).
   The function returns c(n). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Global buffers (static to persist between calls). */
static seq_t *a_seq = NULL;       /* a values */
static seq_t *b_seq = NULL;       /* b values */
static seq_t *c_seq = NULL;       /* c values */
static size_t seq_cap = 0;        /* current array capacity */
static idx_t  seq_len = 0;        /* number of computed terms */

/* Bitmap of used numbers (positive integers). */
static bool *used = NULL;
static size_t used_cap = 0;       /* bitmap capacity (in bool elements) */

/* Expand the bitmap so that it can index at least 'value'. */
static void ensure_used_capacity(idx_t value)
{
    if (used_cap > (size_t)value) return;
    size_t new_cap = used_cap ? used_cap : 64;
    while ((size_t)value >= new_cap) new_cap <<= 1;
    bool *new_used = (bool *)calloc(new_cap, sizeof(bool));
    if (used) {
        memcpy(new_used, used, used_cap * sizeof(bool));
        free(used);
    }
    used = new_used;
    used_cap = new_cap;
}

/* Expand the sequence arrays to at least one more term. */
static void ensure_seq_capacity(void)
{
    if (seq_len < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    b_seq = (seq_t *)realloc(b_seq, new_cap * sizeof(seq_t));
    c_seq = (seq_t *)realloc(c_seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the smallest unused positive integer >= start. */
static idx_t next_unused(idx_t start)
{
    for (;;) {
        ensure_used_capacity(start);
        if (!used[(size_t)start]) return start;
        ++start;
    }
}

/* Initialisation (run once). */
static void initialise(void)
{
    if (seq_len != 0) return;          /* already done */

    ensure_seq_capacity();

    /* a(0) = 1, b(0) = 2, c(0) = 5 */
    a_seq[0] = (seq_t)1;
    b_seq[0] = (seq_t)2;
    c_seq[0] = (seq_t)(1 + 2 * (seq_t)2);

    seq_len = 1;

    /* Mark 1, 2 and 5 as used */
    ensure_used_capacity(5);
    used[1] = true;
    used[2] = true;
    used[5] = true;
}

/* Exported routine. */
seq_t a(idx_t n)
{
    initialise();

    /* Extend precomputation up to n. */
    while (seq_len <= n) {
        idx_t a_val_raw = next_unused(1);
        ensure_used_capacity(a_val_raw);
        used[(size_t)a_val_raw] = true;

        idx_t b_val_raw = next_unused(a_val_raw + 1);
        ensure_used_capacity(b_val_raw);
        used[(size_t)b_val_raw] = true;

        seq_t a_val = (seq_t)a_val_raw;
        seq_t b_val = (seq_t)b_val_raw;
        seq_t c_val = a_val + (seq_t)2 * b_val;

        /* Mark c_val as used.  We assume the value fits in idx_t
           for the intended usage ranges (n up to ~10^4). */
        idx_t c_idx = (idx_t)c_val;
        ensure_used_capacity(c_idx);
        used[(size_t)c_idx] = true;

        ensure_seq_capacity();
        a_seq[seq_len] = a_val;
        b_seq[seq_len] = b_val;
        c_seq[seq_len] = c_val;
        ++seq_len;
    }

    return c_seq[n];
}
