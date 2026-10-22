/*
 * A338981  Number of unoriented colorings of the 120 dodecahedral facets of
 *          the 4-D 120-cell (or 120 vertices of the 4-D 600-cell) using
 *          exactly n colors.
 * https://oeis.org/A338981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, easy
 * Formula:   a(n) = Σ_{j≥n} (num_j * (LCM/den_j) * n! * S₂(j,n)) / LCM, where
 *            the sum is over the 23 terms given in the OEIS entry, S₂(j,n)
 *            are Stirling numbers of the second kind, and LCM = 14400. a(0) =
 *            0 and a(n) = 0 for n>120
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for bool */
#include <stdbool.h>
#include <stddef.h>

/* Term structure matching the OEIS coefficients */
typedef struct {
    unsigned int j;   /* j in bp(j) */
    unsigned int num; /* numerator part of coefficient */
    unsigned int den; /* denominator part of coefficient */
} term_t;

/* The 23 terms from the generating function */
static const term_t TERMS[] = {
    {4,  1, 15 }, {6,  1, 10 }, {8,  1, 15 }, {10, 1, 12 },
    {12, 7, 300}, {16, 1, 50 }, {17, 1, 10 }, {19, 1, 10 },
    {20, 1, 360}, {22, 1, 36 }, {23, 1, 12 }, {24, 7, 300},
    {27, 1, 12 }, {30, 1, 240}, {31, 1, 8  }, {32, 1, 50 },
    {40, 1, 360}, {44, 1, 36 }, {60, 1, 14400},{61, 1, 240},
    {62, 1, 32  }, {75, 1, 240},{120, 1, 14400}
};
#define NUM_TERMS (sizeof TERMS / sizeof TERMS[0])
#define MAX_J 120
#define LCM 14400

/* Internal cache (only initialized once) */
static bool initialized = false;
static seq_t fact_arr[MAX_J + 1];
static seq_t stir[MAX_J + 1][MAX_J + 1];

/* One‑time initialization of factorials and Stirling numbers */
static void init(void)
{
    size_t i, j, k;
    fact_arr[0] = (seq_t)1;
    for (i = 1; i <= (size_t)MAX_J; ++i)
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;

    stir[0][0] = (seq_t)1;
    for (j = 1; j <= (size_t)MAX_J; ++j) {
        for (k = 1; k <= j; ++k)
            stir[j][k] = (seq_t)k * stir[j - 1][k] + stir[j - 1][k - 1];
    }
    initialized = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0 || n > (idx_t)MAX_J)
        return (seq_t)0;

    if (!initialized)
        init();

    /* All indices are now <= MAX_J, safe to convert to size_t */
    size_t un = (size_t)n;
    seq_t result = (seq_t)0;

    for (size_t t = 0; t < (size_t)NUM_TERMS; ++t) {
        const term_t *p = &TERMS[t];
        if (un <= (size_t)p->j) {
            /* n! * S₂(j,n) */
            seq_t temp = fact_arr[un] * stir[p->j][un];
            /* multiply by num * (LCM/den) */
            seq_t factor = (seq_t)p->num * (seq_t)(LCM / p->den);
            temp *= factor;
            result += temp;
        }
    }
    return result / (seq_t)LCM;
}
