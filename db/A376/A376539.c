/*
 * A376539  Natural numbers whose iterated squaring modulo 1000 eventually
 *          settles at the attractor 376.
 * https://oeis.org/A376539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   leads_to_376
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A376539: natural numbers whose iterated squaring modulo 1000 eventually
   settles at the attractor 376.
   The algorithm finds all residues r (0 ≤ r < 1000) that lead to 376 under
   repeated squaring modulo 1000.  Each such residue generates an arithmetic
   progression r + 1000·k (k ≥ 0).  The sequence is the sorted merge of these
   progressions.  The function evaluates the nth term by generating terms from
   the progressions on demand without pre‑computing the entire sequence. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/leads_to_376.h"

/* Global data describing the residue classes that converge to 376. */
static bool initialized = false;
static uint16_t *residues = NULL; /* array of residues */
static size_t resid_cnt = 0;      /* number of residues */

/* Main function to return the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Lazily initialize the residue list once. */
    if (!initialized) {
        static const unsigned int MOD = 1000U;
        uint16_t tmp[MOD];
        size_t cnt = 0;

        for (unsigned int r = 0; r < MOD; ++r) {
            if (leads_to_376(r)) {
                tmp[cnt++] = (uint16_t)r;
            }
        }
        resid_cnt = cnt;
        residues = (uint16_t *)malloc(resid_cnt * sizeof(uint16_t));
        for (size_t i = 0; i < resid_cnt; ++i) {
            residues[i] = tmp[i];
        }
        initialized = true;
    }

    /* Generate terms up to the desired index. */
    seq_t answer = 0;
    size_t count = 0;
    /* k[i] records the multiplier for residues[i] (number of 1000 increments). */
    uint64_t k[1000]; /* 1000 is an upper bound on resid_cnt */
    for (size_t i = 0; i < resid_cnt; ++i) {
        k[i] = 0;
    }

    while (count < (size_t)n) {
        /* Find the smallest next value among all progressions. */
        size_t best_idx = 0;
        seq_t best_val = (seq_t)residues[0] + ((seq_t)1000 * (seq_t)k[0]);
        for (size_t i = 1; i < resid_cnt; ++i) {
            seq_t cand = (seq_t)residues[i] + ((seq_t)1000 * (seq_t)k[i]);
            if (cand < best_val) {
                best_val = cand;
                best_idx = i;
            }
        }

        /* Record the best value. */
        answer = best_val;
        ++count;
        ++k[best_idx];
    }

    return answer;
}
