/*
 * A347198  Consider the sequence of binary words of integers, LSB to MSB S(k)
 *          = ( 0; 1; 01; 11; 001; 101; 011; ... ). Start with a(0) = 0,
 *          extend this sequence with the minimum number of [0;1] such that it
 *          contains S(1) then S(2) and so forth, as sub-strings with LSB at
 *          any a(n). We extend the sequence as m...
 * https://oeis.org/A347198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347198: greedy construction of binary sequence
   containing all reversed binary words S(k) (k ≥ 1) as substrings,
   extending the sequence only when necessary. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static state shared by all calls to a() */
static seq_t *bits = NULL;          /* sequence bits (0 or 1) */
static size_t bits_len = 0;         /* current length of bits */
static size_t bits_cap = 0;         /* allocated capacity of bits */
static idx_t next_k = 1;            /* next integer whose S(next_k) must be added */

/* return true if pattern pat[0..len-1] occurs somewhere in bits[0..bits_len-1] */
static bool contains_pattern(const seq_t *bits, size_t bits_len,
                             const seq_t *pat, idx_t len)
{
    if ((size_t)len > bits_len)
        return false;
    size_t limit = bits_len - (size_t)len + 1;
    for (size_t i = 0; i < limit; ++i) {
        bool ok = true;
        for (size_t j = 0; j < (size_t)len; ++j) {
            if (bits[i + j] != pat[j]) {
                ok = false;
                break;
            }
        }
        if (ok)
            return true;
    }
    return false;
}

/* extend sequence until it contains indices 0 … target */
static void ensure_sequence(idx_t target)
{
    /* first call: initialise sequence with a(0)=0 */
    if (bits == NULL) {
        bits_cap = 16;
        bits = (seq_t *)malloc(bits_cap * sizeof(seq_t));
        if (!bits) abort();
        bits[0] = (seq_t)0;
        bits_len = 1;
        next_k = 1;
    }

    while (bits_len <= (size_t)target) {
        /* build reversed binary pattern for next_k */
        idx_t k = next_k;
        idx_t len = 0;
        idx_t tmp = k;
        do {
            ++len;
            tmp >>= 1;
        } while (tmp > 0);

        seq_t pat[128]; /* max length for 64‑bit integer */
        tmp = k;
        for (idx_t i = 0; i < len; ++i) {
            pat[i] = (seq_t)(tmp & 1u);
            tmp >>= 1;
        }

        /* if the pattern already exists, nothing to append */
        if (!contains_pattern(bits, bits_len, pat, len)) {
            /* find maximum overlap between suffix of bits and prefix of pat */
            idx_t max_overlap = bits_len < (size_t)len ? (idx_t)bits_len : len;
            idx_t l = 0;
            for (idx_t cand = max_overlap; cand > 0; --cand) {
                bool ok = true;
                for (idx_t i = 0; i < cand; ++i) {
                    if (bits[bits_len - cand + i] != pat[i]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    l = cand;
                    break;
                }
            }
            size_t needed = bits_len + (len - l);
            if (needed > bits_cap) {
                size_t new_cap = bits_cap ? bits_cap : 1;
                while (new_cap < needed) new_cap <<= 1;
                seq_t *new_bits = (seq_t *)realloc(bits, new_cap * sizeof(seq_t));
                if (!new_bits) abort();
                bits = new_bits;
                bits_cap = new_cap;
            }
            for (idx_t i = l; i < len; ++i)
                bits[bits_len++] = pat[i];
        }
        ++next_k;
    }
}

/* public function required by OEIS */
seq_t a(idx_t n)
{
    if (bits_len <= (size_t)n)
        ensure_sequence(n);
    return bits[(size_t)n];
}
