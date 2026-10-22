/*
 * A357943  a(0) = 0; a(1) = 1, a(2) = 2; for n > 2, a(n) is the number of
 *          times the term a(n - 1 - a(n-1)) has appeared in the sequence.
 * https://oeis.org/A357943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 917, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 0, 1, 2 for n=0,1,2;
   for n>2   a(n) = number of occurrences of a(n-1-a(n-1)) in the sequence up to index n-1. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq = NULL;          /* generated sequence values      */
static size_t seq_cap = 0;         /* capacity of seq                */
static size_t seq_len = 0;         /* number of elements stored      */

static idx_t *freq = NULL;         /* frequency table for seq_t values */
static size_t freq_cap = 0;        /* capacity of freq               */

/* Ensure seq has room for at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure freq can index 'idx' (i.e. size > idx).  New entries are zeroed. */
static void ensure_freq_cap(idx_t idx)
{
    if ((size_t)idx < freq_cap)
        return;
    size_t new_cap = freq_cap ? freq_cap : 16;
    while ((size_t)new_cap <= (size_t)idx)
        new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(freq, new_cap * sizeof(idx_t));
    if (!tmp)
        abort();
    for (size_t i = freq_cap; i < new_cap; ++i)
        tmp[i] = 0;
    freq = tmp;
    freq_cap = new_cap;
}

/* Append a new element 'val' to the sequence and update frequency. */
static void append_seq_value(seq_t val)
{
    size_t pos = seq_len;
    ensure_seq_cap(pos + 1);

    seq[pos] = val;
    ++seq_len;

    idx_t val_idx = (idx_t)val;          /* values are assumed to fit in idx_t   */
    ensure_freq_cap(val_idx);
    ++freq[val_idx];
}

/* Return a(n).  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n < 0)
        return 0;   /* undefined for negative indices */

    /* Grow the sequence up to index n. */
    while (seq_len <= (size_t)n) {
        size_t m = seq_len;   /* index to compute */

        if (m == 0) {
            append_seq_value((seq_t)0);
        } else if (m == 1) {
            append_seq_value((seq_t)1);
        } else if (m == 2) {
            append_seq_value((seq_t)2);
        } else {
            seq_t prev_val = seq[m - 1];
            idx_t km = (idx_t)(m - 1);
            /* Convert km to seq_t for comparison */
            seq_t km_seq = (seq_t)km;

            idx_t target_idx = 0;
            if (prev_val <= km_seq)
                target_idx = km - (idx_t)prev_val;   /* cast value to idx_t */

            seq_t target_val = seq[target_idx];

            /* Count how many times target_val has appeared so far. */
            ensure_freq_cap((idx_t)target_val);
            idx_t count = freq[(idx_t)target_val];

            append_seq_value((seq_t)count);
        }
    }

    return seq[n];
}
