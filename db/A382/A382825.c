/*
 * A382825  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where A(n,k) = n! * k! * [x^n * y^k] 1 / ( (1-x) *
 *          (1-y) * (1 - log(1-x) * log(1-y))^3 ).
 * https://oeis.org/A382825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A382825 – square array A(n,k) = Σ_{j=0..min(n,k)} (j!)² *
 *            C(j+2,2) * * |Stirling1(n+1,j+1)| * |Stirling1(k+1,j+1)| (read
 *            by antidiagonals)
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* ------------------------------------------------------------ */
/* constants – enough to cover n up to about 10 000            */
/* maximum diagonal index s ≈ 140, so 205 gives a safe margin */
#define MAX_COORD 205

/* ------------------------------------------------------------ */
/* pre‑compute signless Stirling numbers of the first kind      */
/* stored in a flat table S[n][k] (0 ≤ n,k ≤ MAX_COORD)       */
static void compute_stirling(seq_t *S, size_t N)
{
    size_t stride = N + 1;
    S[0 * stride + 0] = 1;            /* S(0,0) = 1 */
    for (size_t n = 1; n <= N; ++n) {
        for (size_t k = 1; k <= n; ++k) {
            seq_t a = S[(n - 1) * stride + (k - 1)];
            seq_t b = S[(n - 1) * stride + k];
            b = b * (seq_t)(n - 1);
            S[n * stride + k] = a + b;
        }
    }
}

/* ------------------------------------------------------------ */
/* pre‑compute factorials up to MAX_COORD                       */
static void compute_factorials(seq_t *fact, size_t N)
{
    fact[0] = 1;
    for (size_t i = 1; i <= N; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
}

/* ------------------------------------------------------------ */
/* convert linear index n into anti‑diagonal coordinates (i,j) */
/* (i increases from 0 downward along the diagonal).           */
static void antidiag_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t s = 0;
    /* find largest s with (s+1)(s+2)/2 <= n */
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    /* number of terms up to previous diagonal */
    idx_t prev = (s * (s + 1)) / 2;
    idx_t offset = n - prev;
    *i = offset;
    *j = s - offset;
}

/* ------------------------------------------------------------ */
/* public sequence function                                      */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t S[MAX_COORD + 1][MAX_COORD + 1];
    static seq_t fact[MAX_COORD + 1];

    if (!initialized) {
        compute_stirling((seq_t *)S, MAX_COORD);
        compute_factorials(fact, MAX_COORD);
        initialized = true;
    }

    idx_t i, j;
    antidiag_coords(n, &i, &j);

    idx_t min_ij = (i < j) ? i : j;
    seq_t sum = 0;

    for (idx_t l = 0; l <= min_ij; ++l) {
        seq_t f   = fact[(size_t)l];
        seq_t term = f * f;                          /* (j!)² */

        /* binomial(l+2,2) = (l+2)(l+1)/2 */
        seq_t bin = ((seq_t)(l + 2) * (seq_t)(l + 1)) / 2;
        term = term * bin;

        term = term * S[(size_t)(i + 1)][(size_t)(l + 1)];
        term = term * S[(size_t)(j + 1)][(size_t)(l + 1)];

        sum = sum + term;
    }

    return sum;
}
