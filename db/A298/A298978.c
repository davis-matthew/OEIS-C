/*
 * A298978  Lexicographic first sequence of positive integers such that
 *          a(n)*a(n+1) has a digit 8, and no term occurs twice.
 * https://oeis.org/A298978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lexicographically first permutation of the positive
 *            integers such that a(n)*a(n+1) contains the digit 8 and every
 *            integer appears exactly once. The function supports arbitrary
 *            calling order and memoises the sequence up to the maximum n
 *            requested
 * Range:     u8 n <= 14, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequence and used number tracking */
static seq_t *seq = NULL;          /* sequence values, indexed from 1 */
static size_t seq_cap = 0;         /* capacity of seq array */
static size_t seq_len = 0;         /* number of terms currently stored */

static bool *used = NULL;          /* used[number] = true if number already used */
static size_t used_cap = 0;        /* current capacity of used array */

/*----- helper: test whether a decimal representation of v contains an 8 -----*/
static bool has_digit8(seq_t v) {
    if (v == 0) return false;
    while (v != 0) {
        if ((v % 10) == 8) return true;
        v /= 10;
    }
    return false;
}

/*----- dynamic array helpers -----*/
static void enlarge_seq(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void enlarge_used(size_t needed) {
    if (needed <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/*----- public function -----*/
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for 0, return 0 for safety */

    /* one‑time initialization */
    if (seq == NULL) {
        enlarge_seq(128);
        enlarge_used(128);
    }

    /* compute values up to n if needed */
    if ((size_t)n > seq_len) {
        /* initialise first term if we haven't yet */
        if (seq_len == 0) {
            seq_len = 1;
            enlarge_seq(seq_len + 1);
            seq[1] = (seq_t)1;
            enlarge_used(2);
            used[1] = true;
        }

        /* build increasingly for each missing index */
        for (idx_t i = (idx_t)(seq_len + 1); i <= n; ++i) {
            seq_t prev = seq[i - 1];
            idx_t cand = 1;
            for (;;) {
                /* ensure we can index the candidate */
                enlarge_used((size_t)cand + 1);

                if (used[(size_t)cand]) { /* already used, skip */
                    ++cand;
                    continue;
                }

                seq_t prod = prev * (seq_t)cand;
                if (has_digit8(prod)) {
                    seq[i] = (seq_t)cand;
                    used[(size_t)cand] = true;
                    break;
                }
                ++cand;
            }
        }
        seq_len = (size_t)n;
    }

    return seq[n];
}
