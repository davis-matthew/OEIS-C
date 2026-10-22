/*
 * A347738  A variant of the inventory sequence: record the number of terms >=
 *          0 thus far in the sequence, then the number of terms >= 1 thus
 *          far, then the number of terms >= 2 thus far, and so on, until a
 *          zero is recorded; the inventory then starts again, recording the
 *          number of terms >= 0, etc.
 * https://oeis.org/A347738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, nice, look
 * Formula:   not stated
 * Range:     u8 n <= 381, u16 n <= 9154, u32 n <= 9154, u64 n <= 9154,
 *            u128 n <= 9154, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Variant inventory sequence A347738
   a(0) = 0;
   for each block: record the number of terms ≥ 0, then ≥ 1, …
   until a 0 is produced; then start a new block.
   This produces the values listed in the OEIS entry. */

#include <stdlib.h>

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t ncap = seq_cap ? seq_cap << 1 : 8;
    while (ncap < needed) ncap <<= 1;

#ifdef __cplusplus
    // Use C++ new/delete for non‑trivial types
    seq_t *tmp = new seq_t[ncap];
    for (size_t i = 0; i < seq_len; ++i) tmp[i] = seq[i];
    delete[] seq;
    seq = tmp;
    seq_cap = ncap;
#else
    seq_t *tmp = (seq_t*)realloc(seq, ncap * sizeof(seq_t));
    seq = tmp;
    seq_cap = ncap;
#endif
}

seq_t a(idx_t n)
{
    while (seq_len <= n) {
        seq_t threshold = 0;
        while (1) {
            seq_t count = 0;
            for (idx_t i = 0; i < seq_len; ++i)
                if (seq[i] >= threshold) ++count;
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = count;
            if (count == 0) break;
            ++threshold;
        }
    }
    return seq[n];
}
