/*
 * A386760  Numbers k such that the number of decimal digits of the Lucas
 *          number L(k) is greater than the number of decimal digits of the
 *          Fibonacci number F(k).
 * https://oeis.org/A386760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386760
   a(n) = k (1‑based) where k is the n‑th integer such that
   the decimal length of the Lucas number L(k) exceeds that of
   the Fibonacci number F(k).  Computed using the
   fractional‑part test <k·α> < β with high‑precision fixed‑point
   arithmetic (α = log10(φ), β = log10(√5)/2).  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* 15‑digit fixed‑point scale (10^15) is sufficient for k ≤ 10000.
   All computations are integer‑only. */
static const uint64_t SCALE  = 1000000000000000ULL; /* 10^15 */

/* α = log10(φ) ≈ 0.20898764024997873
   β = log10(√5) ≈ 0.349485002168009 */
static const uint64_t ALPHA  = 208987640249978ULL;  /* floor(α·SCALE)  */
static const uint64_t BETA   = 349485002168009ULL;  /* floor(β·SCALE)  */

seq_t a(idx_t n)
{
    static bool  initialized      = false;
    static idx_t cur_k            = 1;       /* last k processed   */
    static uint64_t frac          = ALPHA;  /* fractional part of k·α (scaled) */
    static seq_t *terms          = NULL;    /* array of found k's */
    static size_t terms_cap      = 0;
    static size_t terms_len      = 0;

    /* initial allocation of the terms array */
    if (!initialized) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        /* already have cur_k = 1, frac = α, but no term qualifies at k=1 */
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        /* advance to next k */
        cur_k++;
        /* update fractional part of k·α */
        frac += ALPHA;
        if (frac >= SCALE)
            frac -= SCALE;

        /* k satisfies the condition iff frac < β */
        if (frac < BETA) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms,
                                        terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)cur_k;
        }
    }

    return terms[n - 1];
}
