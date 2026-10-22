/*
 * A367292  An inventory sequence: the first row contains the value 1,
 *          subsequent rows contains the number of terms in prior rows whose
 *          binary expansions contain 2^0 (provided this number is nonzero),
 *          then the number of terms for 2^1 (provided it is > 0), then for
 *          2^2, etc.
 * https://oeis.org/A367292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A367292: Inventory sequence
 * a(1) = 1.  For each subsequent row, for bits k = 0,1,2,..., output
 * the count of previous terms that contain bit 2^k, provided that count
 * is non‑zero.  The row stops when a zero count would be appended.
 * The full sequence is the concatenation of all rows.
 * This implementation memoises the sequence and expands it until the
 * required index n is obtained.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t bit_counts[128] = {0};

/* ensure the sequence array can hold at least <need> elements */
static void grow_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq_len == 0) {
        grow_capacity(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        /* update bit counts for the initial element */
        seq_t val = seq[0];
        size_t max_bits = sizeof(seq_t) * CHAR_BIT;
        for (size_t b = 0; b < max_bits; ++b) {
            if ((val >> b) & (seq_t)1)
                bit_counts[b]++;
        }
    }

    /* generate rows until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* build next row */
        seq_t row_buf[128];
        size_t row_len = 0;
        size_t max_bits = sizeof(seq_t) * CHAR_BIT;
        for (size_t b = 0; b < max_bits; ++b) {
            uint64_t cnt = bit_counts[b];
            if (cnt == 0)
                break;
            row_buf[row_len++] = (seq_t)cnt;
        }

        /* append the row to the global sequence */
        grow_capacity(seq_len + row_len);
        for (size_t i = 0; i < row_len; ++i)
            seq[seq_len + i] = row_buf[i];
        seq_len += row_len;

        /* update bit counts with the new terms */
        for (size_t i = 0; i < row_len; ++i) {
            seq_t val = row_buf[i];
            for (size_t b = 0; b < max_bits; ++b) {
                if ((val >> b) & (seq_t)1)
                    bit_counts[b]++;
            }
        }
    }

    return seq[n - 1];
}
