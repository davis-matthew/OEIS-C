/*
 * A370917  Positive integers whose ternary representation includes at least
 *          one 0, and every 0 is followed by 2.
 * https://oeis.org/A370917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose ternary representation
 *            contains at least one 0, and every 0 is immediately followed by
 *            a 2
 * Range:     u8 n <= 36, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Static storage for the sequence values.  The array is grown
   incrementally as more terms are required. */
static seq_t *seq = NULL;
static idx_t seq_cap = 0;
static idx_t seq_size = 0;
static idx_t max_len_done = 0;

/* Ensure space for at least 'needed' elements.  The capacity is
   doubled each time it is increased to keep amortised cost O(1). */
static void ensure_cap(idx_t needed) {
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Recursive generator for all ternary strings of a given length.
   The parameters carry the current position, the value built so far
   and a flag indicating whether a token '02' (i.e., a zero
   followed by 2) has already appeared.  The recursion explores
   tokens in lexicographic order (0 first, then 1, then 2) to
   produce numbers in increasing order. */
static void dfs_len(idx_t len, idx_t pos, seq_t val, bool zero_present) {
    if (pos == len) {
        if (zero_present) {
            if (seq_size >= seq_cap) ensure_cap(seq_size + 1);
            seq[seq_size++] = val;
        }
        return;
    }

    /* Token '02' : two digits, first digit 0 (allowed only not at position 0). */
    if (pos > 0 && len - pos >= 2) {
        seq_t tmp = val * 3;          /* add digit 0  */
        seq_t nxt = tmp * 3 + 2;      /* add digit 2  */
        dfs_len(len, pos + 2, nxt, true);
    }

    /* Token '1'. */
    if (len - pos >= 1) {
        seq_t nxt = val * 3 + 1;
        dfs_len(len, pos + 1, nxt, zero_present);
    }

    /* Token '2'. */
    if (len - pos >= 1) {
        seq_t nxt = val * 3 + 2;
        dfs_len(len, pos + 1, nxt, zero_present);
    }
}

/* Generate all terms of length 'len'.  The calls are made only once
   for each 'len' thanks to the global counter 'max_len_done'. */
static void generate_len(idx_t len) {
    dfs_len(len, 0, (seq_t)0, false);
}

/* Public API – returns the n-th term (1‑based).  The sequence is
   built incrementally and cached between calls, so repeated
   requests for the same or smaller indices are instantaneous. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0 – safe fallback */

    /* Build terms until we have at least n of them. */
    while (seq_size < n) {
        ++max_len_done;
        generate_len(max_len_done);
    }
    return seq[n - 1];
}
