/*
 * A355650  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. exp(x^k/k! * (exp(x) - 1)).
 * https://oeis.org/A355650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth antidiagonal entry of the triangle T(n,k)
 *            defined by T(0,k) = 1 for all k ≥ 0, T(n,k) = n! *
 *            Σ_{j=0}^{⌊n/(k+1)⌋} S(n−k*j , j) / ( (k!)^j * (n−k*j)! ) for n >
 *            0, where S(m,j) is the Stirling number of the second kind. The
 *            sequence is read by antidiagonals, i.e. a(0)=T(0,0), a(1)=T(...
 * Range:     u8 n <= 19, u16 n <= 34, u32 n <= 64, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- Global caches for factorials and Stirling numbers of the second kind --- */

static seq_t *fact = NULL;          /* factorial[0..F]      */
static idx_t fact_n = 0;            /* highest factorial computed   */

static seq_t **S = NULL;            /* Stirling rows 0..S_n    */
static idx_t stir_n = 0;            /* highest Stirling n computed */

/* Ensure factorials are computed up to n */
static void ensure_fact(idx_t n)
{
    if (fact_n >= n) return;
    seq_t *p = (seq_t *)realloc(fact, sizeof(seq_t) * (n + 1));
    if (!p) exit(1);          /* abort on allocation failure */
    fact = p;
    if (fact_n == 0) fact[0] = 1;   /* factorial[0] = 1 */
    for (idx_t i = fact_n + 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    fact_n = n;
}

/* Ensure Stirling numbers S(m,j) are available up to m = n */
static void ensure_stirling(idx_t n)
{
    if (stir_n >= n) return;
    seq_t **p = (seq_t **)realloc(S, sizeof(seq_t *) * (n + 1));
    if (!p) exit(1);
    S = p;
    for (idx_t i = stir_n + 1; i <= n; ++i) {
        S[i] = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!S[i]) exit(1);
        S[i][0] = 0;
        if (i > 0) S[i][i] = 1;
        for (idx_t k = 1; k < i; ++k) {
            S[i][k] = (seq_t)k * S[i - 1][k] + S[i - 1][k - 1];
        }
    }
    stir_n = n;
}

/* Return Stirling number S(m,j); 0 when j>m */
static inline seq_t get_stirling(idx_t m, idx_t j)
{
    if (j > m) return 0;
    ensure_stirling(m);
    return S[m][j];
}

/* Compute T(n,k) using the second formula */
static seq_t T_nk(idx_t n, idx_t k)
{
    if (n == 0) return 1;          /* T(0,k) = 1 for all k */
    ensure_fact(n);
    seq_t result = 0;
    idx_t jmax = n / (k + 1);
    seq_t factk = fact[k];   /* k! */
    seq_t pow_factk = 1;     /* (k!)^j, start with j=0 */

    for (idx_t j = 0; j <= jmax; ++j) {
        idx_t m = n - k * j;          /* m = n – k*j */
        seq_t Smj = get_stirling(m, j);
        if (Smj == 0) { /* term will be zero, skip division */
            pow_factk = pow_factk * factk;
            continue;
        }
        seq_t num = fact[n] * Smj;          /* n! * S(m,j) */
        seq_t den = fact[m] * pow_factk;    /* m! * (k!)^j */
        seq_t term = num / den;             /* exact division */
        result += term;
        pow_factk *= factk;                 /* update for next j */
    }
    return result;
}

/* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
static inline idx_t diagonal_of(idx_t n)
{
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    return d;
}

/* Main function as requested */
seq_t a(idx_t n)
{
    idx_t d = diagonal_of(n);
    idx_t start_of_diag = d * (d + 1) / 2;   /* index of first element on diagonal d */
    idx_t offset = n - start_of_diag;        /* 0..d */
    idx_t n_prime = offset;                  /* row index  */
    idx_t k_prime = d - offset;              /* column index */
    /* Ensure caches large enough for this T(n',k') */
    idx_t max_m = (n_prime > k_prime) ? n_prime : k_prime;
    ensure_fact(max_m);
    ensure_stirling(max_m);
    return T_nk(n_prime, k_prime);
}
