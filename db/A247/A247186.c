/*
 * A247186  a(0) = 0. a(n) is the number of repeating sums in the collection
 *          of all sums formed from [a(0), ... a(n-1)] + [a(0), ... a(n-1)].
 * https://oeis.org/A247186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of repeating sums in the collection of all sums
 *            formed from [a(0), ... a(n-1)] + [a(0), ... a(n-1)]
 * Range:     u8 n <= 23, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static storage for computed terms and auxiliary structures */
    static seq_t *seq = NULL;          /* computed sequence values */
    static size_t seq_len = 0, seq_cap = 0;

    static seq_t *uniq = NULL;          /* array of seen unique terms */
    static size_t uniq_len = 0, uniq_cap = 0;

    static uint64_t *bits = NULL;       /* bitset of achievable sums */
    static size_t bits_len = 0;          /* number of 64‑bit words */
    static size_t distinct_sum_count = 0;/* number of distinct sums */

    /* ----- initialization for a(0) ----- */
    if (seq_len == 0) {
        /* sequence array: start with capacity 1 */
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;
        seq_len = 1;

        /* unique values array: start with one element (0) */
        uniq_cap = 8;
        uniq = (seq_t *)malloc(uniq_cap * sizeof(seq_t));
        uniq[0] = 0;
        uniq_len = 1;

        /* sum bitset: at least one bit (for 0+0) */
        bits_len = 1;
        bits = (uint64_t *)malloc(bits_len * sizeof(uint64_t));
        bits[0] = 1ULL;                /* bit 0 set */
        distinct_sum_count = 1;        /* only sum 0 exists */
    }

    /* ----- compute terms up to n ----- */
    while (seq_len <= (size_t)n) {
        /* Compute a(seq_len) using previously built state */
        seq_t total_pairs = (seq_t)seq_len * ((seq_t)seq_len + 1) / 2;
        seq_t repeats = total_pairs - (seq_t)distinct_sum_count;

        /* ensure capacity for new term */
        if (seq_len >= seq_cap) {
            seq_cap = seq_cap * 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len] = repeats;
        seq_t val = repeats;           /* new element to add to the system */

        /* ---------- update distinct‑sum set if val is new ---------- */
        int is_new = 1;
        for (size_t i = 0; i < uniq_len; ++i) {
            if (uniq[i] == val) {
                is_new = 0;
                break;
            }
        }

        if (is_new) {
            /* sums of val with all existing unique values */
            for (size_t i = 0; i < uniq_len; ++i) {
                seq_t s = val + uniq[i];
                uint64_t idx = (uint64_t)s;
                size_t word = idx >> 6;
                uint64_t mask = 1ULL << (idx & 63);

                if (word >= bits_len) {
                    size_t old_len = bits_len;
                    size_t new_len = word + 1;
                    uint64_t *new_bits = (uint64_t *)realloc(bits, new_len * sizeof(uint64_t));
                    for (size_t j = old_len; j < new_len; ++j) new_bits[j] = 0;
                    bits = new_bits;
                    bits_len = new_len;
                }

                if ((bits[word] & mask) == 0) {
                    bits[word] |= mask;
                    ++distinct_sum_count;
                }
            }

            /* self‑sum (val + val) */
            seq_t s = val + val;
            uint64_t idx = (uint64_t)s;
            size_t word = idx >> 6;
            uint64_t mask = 1ULL << (idx & 63);

            if (word >= bits_len) {
                size_t old_len = bits_len;
                size_t new_len = word + 1;
                uint64_t *new_bits = (uint64_t *)realloc(bits, new_len * sizeof(uint64_t));
                for (size_t j = old_len; j < new_len; ++j) new_bits[j] = 0;
                bits = new_bits;
                bits_len = new_len;
            }

            if ((bits[word] & mask) == 0) {
                bits[word] |= mask;
                ++distinct_sum_count;
            }

            /* add value to unique list */
            if (uniq_len >= uniq_cap) {
                uniq_cap = uniq_cap * 2;
                uniq = (seq_t *)realloc(uniq, uniq_cap * sizeof(seq_t));
            }
            uniq[uniq_len++] = val;
        }

        ++seq_len;     /* ready for next n */
    }

    return seq[(size_t)n];
}
