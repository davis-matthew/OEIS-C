/*
 * A258366  Numbers n representable as x*y + x + y, where x >= y > 1, such
 *          that all x's and y's in all representation(s) of n are perfect
 *          squares.
 * https://oeis.org/A258366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate terms by checking each integer n,
   testing that for all divisor pairs (x+1, y+1) of n+1 with
   y+1 ≥ 3, both x and y are perfect squares. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_seq.h"

static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static seq_t last_checked = 0;       /* last integer n tested */

static inline bool is_square_minus_one(seq_t v) {
    /* v must be ≥ 3; return true if v-1 is a perfect square. */
    seq_t w = v - (seq_t)1;
    seq_t r = isqrt_seq(w);
    return r * r == w;
}

static inline bool ok_term(seq_t n) {
    seq_t N = n + (seq_t)1;                      /* (x+1)(y+1) */
    seq_t sqrtN_seq = isqrt_seq(N);
    uint64_t sqrtN = (uint64_t)sqrtN_seq;        /* safe for loop bound */
    bool has_pair   = false;
    for (uint64_t d = 3; d <= sqrtN; ++d) {
        seq_t d_seq = (seq_t)d;
        if (N % d_seq != (seq_t)0) continue;     /* not a divisor */
        seq_t e_seq = N / d_seq;
        if (e_seq < (seq_t)3) continue;          /* y+1 must be ≥3 */
        if (!is_square_minus_one(d_seq)) {       /* x not square */
            return false;
        }
        if (!is_square_minus_one(e_seq)) {       /* y not square */
            return false;
        }
        has_pair = true;                         /* found a valid pair */
    }
    return has_pair;                              /* must have at least one */
}

static void ensure_terms_size(void) {
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = new_ptr;
        terms_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for offset 1 */
    while (terms_len < (size_t)n) {
        seq_t cand = last_checked + (seq_t)1;
        last_checked = cand;
        if (ok_term(cand)) {
            ensure_terms_size();
            terms[terms_len++] = cand;
        }
    }
    return terms[(size_t)n - 1];
}
