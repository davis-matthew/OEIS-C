/*
 * A282888  Lexicographically earliest permutation p of the nonnegative
 *          integers such that Sum_{k=0..n} -(-1)^issquare(p(k))*p(k) is
 *          nonnegative for all n.
 * https://oeis.org/A282888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Lexicographically earliest permutation p of the nonnegative *
 *            integers such that the cumulative sum S(n) = Σ_{k=0..n}
 *            -(-1)^issquare(p(k))*p(k) * remains non‑negative. The sequence
 *            a(n) = p(n). * Construction: at each step choose the smallest
 *            unused integer that is legal – * a square (always lega...
 * Range:     u8 n <= 60, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_square_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/is_square_idx.h"

/* Dynamic array for the permutation p(k). */
static seq_t *perm = NULL;
static idx_t  perm_cap = 0, perm_len = 0;

/* Bitmap of used values. */
static uint8_t *used = NULL;
static size_t used_cap = 0;

/* Ensure used[] can store index 'idx' (0‑based) */
static void ensure_used_capacity(idx_t idx) {
    while ((size_t)idx >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        while (new_cap <= (size_t)idx) new_cap <<= 1;
        uint8_t *p = (uint8_t*)realloc(used, new_cap * sizeof(uint8_t));
        if (!p) abort();
        memset(p + used_cap, 0, new_cap - used_cap);
        used = p;
        used_cap = new_cap;
    }
}

/* Ensure perm[] can store element 'pos' (0‑based). */
static void ensure_perm_capacity(idx_t pos) {
    while ((size_t)pos >= perm_cap) {
        size_t new_cap = perm_cap ? perm_cap * 2 : 16;
        seq_t *p = (seq_t*)realloc(perm, new_cap * sizeof(seq_t));
        if (!p) abort();
        perm = p;
        perm_cap = new_cap;
    }
}

/* Global state for constructing the permutation. */
static idx_t next_sq_root = 0;      /* root of the next unused square    */
static idx_t next_sq_val  = 0;      /* value of that square              */
static idx_t next_non_sq  = 0;      /* smallest unused nonsquare         */
static seq_t running_sum = 0;       /* current partial sum S(n)          */

/* ------------------------------------------------------------------ */
/* Public function requested by the problem.                          */
seq_t a(idx_t n) {
    /* One‑time initialization. */
    if (!perm) {
        perm      = (seq_t*)malloc(16 * sizeof(seq_t));
        perm_cap  = 16;
        perm_len  = 1;
        perm[0]   = 0;

        used      = (uint8_t*)calloc(16, sizeof(uint8_t));
        used_cap  = 16;
        used[0]   = 1;                /* value 0 is used */

        running_sum = 0;

        /* first unused square: 1 (root 1) */
        next_sq_root = 1;
        next_sq_val  = 1;

        /* initialise next_non_sq to the first unused nonsquare (>0) */
        next_non_sq = 1;
        while (true) {
            ensure_used_capacity((idx_t)next_non_sq);
            if (!used[(size_t)next_non_sq] && !is_square_idx((idx_t)next_non_sq))
                break;
            ++next_non_sq;
        }
    }

    /* Generate terms up to index n if needed. */
    while (perm_len <= n) {
        /* Choose the smallest legal candidate. */
        seq_t chosen;
        bool square_used = false;

        /* Candidate values.  Root values are idx_t; cast to seq_t when storing. */
        idx_t cand_sq  = next_sq_val;
        idx_t cand_non = next_non_sq;

        if (cand_non <= running_sum && cand_non < cand_sq) {
            chosen = (seq_t)cand_non;
            square_used = false;
        } else {
            chosen = (seq_t)cand_sq;
            square_used = true;
        }

        /* Mark chosen value as used. */
        ensure_used_capacity((idx_t)chosen);
        used[(size_t)chosen] = 1;

        /* Update running sum. */
        if (square_used)
            running_sum += chosen;
        else
            running_sum -= chosen;

        /* Store the constructed term. */
        ensure_perm_capacity(perm_len);
        perm[perm_len++] = chosen;

        /* Advance pointers for the next step. */
        if (square_used) {
            /* Next square: increment root. */
            ++next_sq_root;
            next_sq_val = (idx_t)((__uint128_t)next_sq_root * next_sq_root);
        } else {
            /* Advance to the next unused nonsquare. */
            do {
                ++next_non_sq;
                ensure_used_capacity((idx_t)next_non_sq);
            } while (used[(size_t)next_non_sq] || is_square_idx((idx_t)next_non_sq));
        }
    }

    return perm[n];
}
