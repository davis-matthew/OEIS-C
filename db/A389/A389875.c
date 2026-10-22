/*
 * A389875  Arrangement of numbers such that every digit rotation of the
 *          number n is included in descending order after those of n-1.
 * https://oeis.org/A389875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, easy, look
 * Formula:   a(n) is the n-th element in the infinite concatenation * of, for
 *            each m ≥ 0, the decimal digit rotations of m (including *
 *            duplicates) sorted in descending order
 * Range:     u8 n <= 198, u16 n <= 9160, u32 n <= 9160, u64 n <= 9160,
 *            u128 n <= 9160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;          /* allocated capacity */
static size_t seq_len = 0;          /* number of elements stored */
static idx_t next_m = 0;            /* next m to process */

/* Append a value to the cache, expanding if necessary. */
static void append_to_cache(seq_t val) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        seq_cache = tmp;
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* OEIS A389875 */
seq_t a(idx_t n) {
    /* Build the sequence up to the required index. */
    while (seq_len <= n) {
        if (next_m == 0) {
            /* Special case: single rotation of 0. */
            append_to_cache((seq_t)0);
            ++next_m;
            continue;
        }

        /* Extract decimal digits of next_m. */
        uint8_t digits[20];
        size_t dlen = 0;
        idx_t tmp = next_m;
        while (tmp > 0) {
            digits[dlen++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        /* Reverse to most‑significant‑first order. */
        for (size_t i = 0; i < dlen / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[dlen - 1 - i];
            digits[dlen - 1 - i] = t;
        }

        /* Generate all rotations (including duplicates). */
        seq_t rot[20];
        for (size_t shift = 0; shift < dlen; ++shift) {
            seq_t val = 0;
            for (size_t j = 0; j < dlen; ++j) {
                val = val * 10 + digits[(j + shift) % dlen];
            }
            rot[shift] = val;
        }

        /* Sort rotations in non‑ascending order. */
        for (size_t i = 0; i < dlen; ++i) {
            for (size_t j = i + 1; j < dlen; ++j) {
                if (rot[i] < rot[j]) {
                    seq_t tmpv = rot[i];
                    rot[i] = rot[j];
                    rot[j] = tmpv;
                }
            }
        }

        /* Append sorted rotations to the cache. */
        for (size_t k = 0; k < dlen; ++k) {
            append_to_cache(rot[k]);
        }

        ++next_m;
    }
    return seq_cache[n];
}
