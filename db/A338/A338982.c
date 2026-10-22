/*
 * A338982  Number of chiral pairs of colorings of the 120 dodecahedral facets
 *          of the 4-D 120-cell (or 120 vertices of the 4-D 600-cell) using
 *          exactly n colors.
 * https://oeis.org/A338982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, easy
 * Formula:   a(n) = (1/14400) * Σ coeff_j * n! * S2(j,n) for 0 ≤ n ≤ 120
 *            where S2(j,k) is Stirling numbers of the second kind, coeff_j =
 *            14400 / d * m with d the denominator and m the multiplier in the
 *            generating function. For n > 120, a(n) = 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factorial.h"

static bool initialized = false;
static seq_t fact[121];
static seq_t S2[121][121];

static void init(void) {
    if (initialized) return;
    initialized = true;

    /* factorials */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= 120; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Stirling numbers of the second kind */
    S2[0][0] = (seq_t)1;
    for (idx_t j = 1; j <= 120; ++j) {
        S2[j][0] = (seq_t)0;
        for (idx_t k = 1; k < j; ++k) {
            seq_t a = S2[j - 1][k - 1];
            seq_t b = S2[j - 1][k] * (seq_t)k;
            S2[j][k] = a + b;
        }
        S2[j][j] = (seq_t)1;
    }
}

/* Each term: positive coefficient (no sign), the sign separately, and j. */
struct term_t {
    unsigned int coeff;  /* 14400/denominator * multiplier (positive) */
    int          sign;   /* +1 or -1 */
    int          j;      /* argument to bp() */
};

static const struct term_t terms[] = {
    { 960,  +1, 4 },
    { 1440, +1, 6 },
    { 960,  +1, 8 },
    { 1200, +1,10 },
    { 336,  +1,12 },
    { 288,  +1,16 },
    { 1440, -1,17 },
    { 1440, -1,19 },
    { 40,   +1,20 },
    { 400,  +1,22 },
    { 1200, -1,23 },
    { 336,  +1,24 },
    { 1200, -1,27 },
    { 60,   +1,30 },
    { 1800, -1,31 },
    { 288,  +1,32 },
    { 40,   +1,40 },
    { 400,  +1,44 },
    { 1,    +1,60 },
    { 60,   -1,61 },
    { 450,  +1,62 },
    { 60,   -1,75 },
    { 1,    +1,120 }
};

seq_t a(idx_t n) {
    if (n > 120) return (seq_t)0;
    init();

    seq_t fn = factorial(n);    /* n! */

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (size_t i = 0; i < sizeof terms / sizeof terms[0]; ++i) {
        if (n > (idx_t)terms[i].j) continue;   /* term contributes 0 */
        seq_t prod = fn * S2[terms[i].j][n];
        prod *= (seq_t)terms[i].coeff;
        if (terms[i].sign > 0)
            pos_sum += prod;
        else
            neg_sum += prod;
    }

    seq_t numerator = pos_sum;
    numerator -= neg_sum;       /* pos_sum >= neg_sum for valid a(n) */
    return numerator / (seq_t)14400;
}
