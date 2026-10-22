/*
 * A370400  Lexicographically earliest sequence of distinct nonnegative terms
 *          such that the last digit of a(n) is present in a(n+1).
 * https://oeis.org/A370400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of distinct nonnegative terms
   such that the last digit of a(n) is present in a(n+1). */

#include <stdlib.h>
#include <stdbool.h>

/* Memoized storage for the sequence values */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* Boolean array indicating whether a number has already been used */
static bool *used = NULL;
static size_t used_cap = 0;

/* Next candidate index for each last digit 0..9 */
static idx_t nextCandidate[10] = {0};

/* Ensure the used array can address indices up to 'needed' (inclusive). */
static void ensure_used_cap(idx_t needed) {
    if (needed < used_cap) return;
    idx_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap <= needed) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(1);
    used = tmp;
    /* initialise new entries to false */
    memset(&used[used_cap], 0, (new_cap - used_cap) * sizeof(bool));
    used_cap = new_cap;
}

/* Ensure the seq array can hold at least 'needed' elements. */
static void ensure_seq_cap(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq = tmp;
    /* initialise new entries to 0 (seq_t may be struct, zeroing is safe) */
    memset(&seq[seq_cap], 0, (new_cap - seq_cap) * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Check whether number x contains digit d in its decimal representation. */
static inline bool contains_digit(idx_t x, int d) {
    if (x == 0) return d == 0;
    while (x) {
        if (x % 10 == d) return true;
        x /= 10;
    }
    return false;
}

/* Generate the sequence up to index n (1‑based). */
static void generate(seq_t *out, idx_t n) {
    if (seq == NULL) {
        ensure_seq_cap(1);
        ensure_used_cap(1);
        /* a(1) = 0 */
        seq[0] = 0;
        used[0] = true;
        seq_len = 1;
    }
    if (n <= seq_len) *out = seq[n-1];
    else {
        /* Extend sequence to required n */
        for (size_t i = seq_len; i < (size_t)n; ++i) {
            seq_t prev = seq[i-1];
            int d = (int)(prev % 10);
            idx_t cand = nextCandidate[d];
            /* Search for the smallest unused number containing digit d */
            while (true) {
                ensure_used_cap(cand);
                if (!used[cand] && contains_digit(cand, d)) break;
                ++cand;
            }
            // Store new term
            ensure_seq_cap(i+1);
            seq[i] = cand;
            /* Mark used and update next candidate */
            ensure_used_cap(cand);
            used[cand] = true;
            nextCandidate[d] = cand + 1;
        }
        seq_len = n;
        *out = seq[n-1];
    }
}

/* Public entry point */
seq_t a(idx_t n) {
    seq_t res;
    generate(&res, n);
    return res;
}
