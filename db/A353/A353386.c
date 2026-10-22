/*
 * A353386  Sums of two squares obtained by expanding a k X k matrix to (k+1)
 *          X (k+1) and taking the not yet seen squared distances of all
 *          positions in the matrix.
 * https://oeis.org/A353386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353386
   Generate distinct sums of two squares by iterating k = 1,2,...
   For each k emit k^2 + i^2 for i = 0..k, in that order, skipping
   numbers already produced.  This yields a permutation of the
   positive sums of two squares. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for non‑positive n */

#ifdef __cplusplus
    /* C++ implementation: use std::vector for automatic memory management. */
    #include <vector>
    static std::vector<seq_t> seq;   /* sequence terms */
    static idx_t current_k = 1;     /* current k to expand */

    while (seq.size() < (size_t)n) {
        for (idx_t i = 0; i <= current_k; ++i) {
            seq_t val = (seq_t)i * (seq_t)i + (seq_t)current_k * (seq_t)current_k;
            /* check for duplicate */
            bool seen = false;
            for (size_t j = 0; j < seq.size(); ++j) {
                if (seq[j] == val) { seen = true; break; }
            }
            if (!seen) {
                seq.push_back(val);
                if (seq.size() >= (size_t)n) break;
            }
        }
        if (seq.size() >= (size_t)n) break;
        ++current_k;
    }
    return seq[n - 1];
#else
    /* C implementation: static dynamic array. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t current_k = 1;

    /* Ensure capacity for new element(s). */
    if (seq_len + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 8;
        while (new_cap < seq_len + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }

    while (seq_len < (size_t)n) {
        for (idx_t i = 0; i <= current_k; ++i) {
            seq_t val = (seq_t)i * (seq_t)i + (seq_t)current_k * (seq_t)current_k;
            /* linear duplicate check */
            bool seen = false;
            for (size_t j = 0; j < seq_len; ++j) {
                if (seq[j] == val) { seen = true; break; }
            }
            if (!seen) {
                seq[seq_len++] = val;
                if (seq_len >= (size_t)n) break;
            }
        }
        if (seq_len >= (size_t)n) break;
        ++current_k;
    }
    return seq[n - 1];
#endif
}
