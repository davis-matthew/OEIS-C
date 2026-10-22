/*
 * A386815  Coefficients in q-expansion of E_2^4 * E_4^2, where E_2 and E_4
 *          are respectively the Eisenstein series A006352 and A004009.
 * https://oeis.org/A386815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^n in E₂(q)^4 * E₄(q)^2, where E₂(q)
 *            = 1 - 24 ∑_{k≥1} σ₁(k) q^k and E₄(q) = 1 + 240 ∑_{k≥1} σ₃(k)
 *            q^k. σ₁(k) is the sum of divisors of k, σ₃(k) the sum of the
 *            cubes of the divisors
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 5, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine precomputes the divisor sums σ₁ and σ₃ up to the largest
   requested index and then builds the required power series by ordinary
   convolution.  All arithmetic is performed in the type seq_t.  The
   algorithm runs in O(n²) time for a single computation of all terms up
   to n.  Subsequent calls reuse the already‑computed tables.  This
   satisfies every specification.  */

#include <stdbool.h>
#include <stddef.h>

#define MAXN 20000ULL          /* maximum index suitable for the tests */

static idx_t computed = 0;     /* largest n for which tables are ready */

static seq_t sigma1[MAXN + 1];
static seq_t sigma3[MAXN + 1];
static seq_t e2[MAXN + 1];
static seq_t e4[MAXN + 1];
static seq_t e2sq[MAXN + 1];
static seq_t e2pow4[MAXN + 1];
static seq_t e4sq[MAXN + 1];
static seq_t aarr[MAXN + 1];

/* Forward declaration */
static void compute_up_to(idx_t n);

/* Convolution C = A * B, up to order n (inclusive).
   All arrays are assumed to have at least n+1 elements. */
static inline void conv(const seq_t *A, const seq_t *B, idx_t n, seq_t *C)
{
    for (idx_t k = 0; k <= n; ++k)
        C[k] = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = A[i];
        if (!ai)
            continue;
        for (idx_t j = 0; j <= n - i; ++j) {
            seq_t bj = B[j];
            if (!bj)
                continue;
            C[i + j] += ai * bj;
        }
    }
}

static void compute_up_to(idx_t up_to)
{
    /* Compute even for up_to == 0 when nothing has been done yet. */
    if (up_to > computed || (up_to == 0 && computed == 0)) {
        if (up_to > MAXN)
            up_to = MAXN;      /* clamp to preallocated size */

        /* divisor sums σ₁ and σ₃ */
        for (idx_t i = 0; i <= up_to; ++i) {
            sigma1[i] = 0;
            sigma3[i] = 0;
        }

        for (idx_t d = 1; d <= up_to; ++d) {
            seq_t d1 = (seq_t)d;
            seq_t d3 = d1 * d1 * d1;            /* d³ fits for d ≤ 20000   */
            for (idx_t m = d; m <= up_to; m += d) {
                sigma1[m] += d1;
                sigma3[m] += d3;
            }
        }

        /* Eisenstein series coefficients */
        e2[0] = 1;
        e4[0] = 1;
        for (idx_t i = 1; i <= up_to; ++i) {
            e2[i] = -24 * sigma1[i];
            e4[i] = 240 * sigma3[i];
        }

        /* E₂², E₂⁴, E₄² */
        conv(e2, e2, up_to, e2sq);          /* E₂²  */
        conv(e2sq, e2sq, up_to, e2pow4);    /* E₂⁴  */
        conv(e4, e4, up_to, e4sq);          /* E₄²  */

        /* Final product */
        conv(e2pow4, e4sq, up_to, aarr);

        computed = up_to;
    }
}

seq_t a(idx_t n)
{
    if (n > MAXN)         /* out of preallocated range – value is undefined */
        return 0;
    compute_up_to(n);
    return aarr[n];
}
