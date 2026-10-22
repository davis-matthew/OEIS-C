/*
 * A400658  Lexicographically earliest sequence of distinct natural numbers
 *          such that the concatenation of a(n-1) with a(n) is divisible by
 *          the sum of their digits and no quotient repeats.
 * https://oeis.org/A400658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, new
 * Formula:   Lexicographically earliest permutation of ℕ such that for all
 *            n≥2, concat(a(n-1),a(n)) is divisible by
 *            digit_sum(a(n-1))+digit_sum(a(n)) and the quotients are all
 *            distinct. a(1)=0. (OEIS A400658)
 * Range:     u8 n <= 4, u16 n <= 133, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   concat_parts, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/concat_parts.h"   /* seq_t concat_parts(idx_t a, idx_t b); */
#include "../utils/digit_sum.h"     /* seq_t digit_sum(seq_t v); */

seq_t a(idx_t n)
{
    /*-------------------------------------------------------------
     *  Memoised sequence data
     *-------------------------------------------------------------*/
    static seq_t *seq      = NULL;   /* a(1) .. a(seq_len)  (1‑based)   */
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;

    /*-------------------------------------------------------------
     *  Used numbers table
     *-------------------------------------------------------------*/
    static bool *used_num  = NULL;   /* used_num[i] == true if i already used */
    static size_t used_cap = 0;

    /*-------------------------------------------------------------
     *  Quotient table (to guarantee uniqueness)
     *-------------------------------------------------------------*/
    static seq_t *used_q   = NULL;   /* array of seen quotients   */
    static size_t q_len    = 0;
    static size_t q_cap    = 0;

    /*-------------------------------------------------------------
     *  Initialise on first call
     *-------------------------------------------------------------*/
    if (seq_len == 0) {
        /* initial capacity */
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);

        /* a(1) = 0 */
        seq[0] = (seq_t)0;
        seq_len = 1;

        /* used_num[0] = true */
        used_cap = 1;
        used_num = (bool *)malloc(used_cap * sizeof(bool));
        if (!used_num) exit(1);
        used_num[0] = true;

        /* no quotients yet */
        used_q = NULL;
        q_cap  = 0;
    }

    /*-------------------------------------------------------------
     *  Return already computed value
     *-------------------------------------------------------------*/
    if (n == 0) return (seq_t)0;                 /* guard for accidental 0 index */
    if (seq_len >= (size_t)n) return seq[(size_t)n-1];

    /*-------------------------------------------------------------
     *  Build sequence up to requested index
     *-------------------------------------------------------------*/
    while (seq_len < (size_t)n) {
        /* previous term as seq_t */
        seq_t prev = seq[seq_len-1];

        /* Search for smallest unused candidate */
        idx_t best_candidate = (idx_t)-1;     /* will be set on success */
        seq_t best_Q = 0;

        for (idx_t cand = 0; ; ++cand) {
            /* Ensure used_num resized */
            if ((size_t)cand >= used_cap) {
                size_t new_cap = (size_t)cand + 1;
                bool *tmp = (bool *)realloc(used_num, new_cap * sizeof(bool));
                if (!tmp) exit(1);
                /* initialise new slots */
                for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
                used_num = tmp;
                used_cap = new_cap;
            }
            if (used_num[(size_t)cand]) continue;   /* already used */

            /* Candidate value as seq_t for digit_sum */
            seq_t cand_val = (seq_t)cand;

            /* Digit sums */
            seq_t sumdig = digit_sum(prev) + digit_sum(cand_val);
            if (sumdig == (seq_t)0) continue;        /* division by zero */

            /* Concatenation (cast prev to idx_t – safe for n≤10000) */
            seq_t cat = concat_parts((idx_t)prev, cand);

            if (cat % sumdig != (seq_t)0) continue;  /* not divisible */

            seq_t Q = cat / sumdig;

            /* Check quotient uniqueness */
            bool dup = false;
            for (size_t j = 0; j < q_len; ++j) {
                if (used_q[j] == Q) { dup = true; break; }
            }
            if (dup) continue;                       /* already seen */

            /* Found suitable candidate */
            best_candidate = cand;
            best_Q = Q;
            break;
        }

        /* Append new term */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap ? seq_cap*2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq = tmp;
        }
        seq[seq_len++] = (seq_t)best_candidate;

        /* Mark number as used */
        used_num[(size_t)best_candidate] = true;

        /* Store quotient */
        if (q_len == q_cap) {
            q_cap = q_cap ? q_cap*2 : 4;
            seq_t *tmp = (seq_t *)realloc(used_q, q_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            used_q = tmp;
        }
        used_q[q_len++] = best_Q;
    }

    return seq[(size_t)n-1];
}
