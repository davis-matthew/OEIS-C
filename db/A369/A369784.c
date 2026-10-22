/*
 * A369784  Expansion of e.g.f. exp( (exp(2*(exp(x)-1))-1)/2 ).
 * https://oeis.org/A369784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binom, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369784: a(n) = Sum_{k=0..n} Stirling2(n,k) * A004211(k) */
/* A004211(k) is defined by the EGF exp((exp(2*t)-1)/2).  */
/* The following routine computes a(n) for any n ≥ 0 using the recurrence */
#ifndef SEQ_A369784_H
#define SEQ_A369784_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/binom.h"
#include "../utils/mul_small.h"

/* External declarations */
seq_t a(idx_t n);

static idx_t current_max = 0;
static seq_t *B = NULL;          /* A004211 values up to current_max */
static seq_t *S = NULL;          /* Stirling2 row for current n (size current_max+1) */
static seq_t *a_cache = NULL;    /* cached a(n) up to current_max */
static seq_t *pow2 = NULL;       /* powers of 2 up to current_max */
static size_t alloc_size = 0;

/* Reallocate all arrays to new_size (>= alloc_size) */
static void ensure_capacity(size_t new_size) {
    if (new_size <= alloc_size) return;
    alloc_size = new_size;
    B          = (seq_t *)realloc(B,                         (alloc_size + 1) * sizeof(seq_t));
    S          = (seq_t *)realloc(S,                         (alloc_size + 1) * sizeof(seq_t));
    a_cache    = (seq_t *)realloc(a_cache,                   (alloc_size + 1) * sizeof(seq_t));
    pow2       = (seq_t *)realloc(pow2,                      (alloc_size + 1) * sizeof(seq_t));
    for (size_t i = alloc_size; i <= new_size; ++i) {
        B[i] = 0;
        S[i] = 0;
        a_cache[i] = 0;
        pow2[i] = 0;
    }
    pow2[0] = 1;                       /* 2^0 */
}

/* Compute values up to target n */
static void expand_to(idx_t target) {
    if (target <= current_max) return;

    ensure_capacity((size_t)target);
    /* Fill pow2 up to target */
    for (size_t i = 1; i <= (size_t)target; ++i)
        pow2[i] = mul_small(pow2[i - 1], 2);

    /* Compute A004211 values B[1..target] */
    if (B[0] == 0) B[0] = 1;           /* B[0] = 1 */
    for (idx_t m = current_max + 1; m <= target; ++m) {
        seq_t sum = 0;
        for (idx_t i = 0; i < m; ++i) {
            seq_t bin = binom((idx_t)(m - 1), i);
            seq_t part = seq_mul(pow2[(size_t)(m - 1 - i)], B[i]);
            seq_t prod = seq_mul(bin, part);
            sum += prod;
        }
        B[m] = sum;
    }

    /* Compute Stirling2 rows and a_cache incrementally */
    for (idx_t m = current_max + 1; m <= target; ++m) {
        if (m == 1) {
            S[1] = 1;          /* S(1,1)=1 */
        } else {
            for (idx_t k = m; k >= 1; --k) {
                seq_t term = seq_mul((seq_t)k, S[k]);   /* k * S(prev, k) */
                term += S[k - 1];                       /* + S(prev, k-1) */
                S[k] = term;
            }
        }
        S[0] = 0;                      /* S(m,0)=0 for m>0 */
        /* Compute a(m) */
        seq_t an = 0;
        for (idx_t k = 0; k <= m; ++k)
            an += seq_mul(S[k], B[k]);
        a_cache[m] = an;
    }

    a_cache[0] = 1;                  /* a(0)=1 */
    current_max = target;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    expand_to(n);
    return a_cache[n];
}

#endif /* SEQ_A369784_H */
