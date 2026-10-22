/*
 * A355423  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. exp(Sum_{j=1..k} (exp(j*x) -
 *          1)).
 * https://oeis.org/A355423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,k) = 1 * T(n,k) = Σ_{i=1..n} ( Σ_{j=1..k} j^i ) ·
 *            binomial(n-1,i-1) · T(n-i,k) * Sequence a(n) is read from the
 *            square array T(n,k) along antidiagonals * (n + k = constant,
 *            with n increasing). For example: a(0)=1, a(1)=1, a(2)=0, *
 *            a(3)=1, a(4)=1, a(5)=0, …
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Maximum dimensions needed for n up to ~10000 */
#define MAXDIM 200

/* Static storage reused for each call */
static seq_t powv[MAXDIM + 1][MAXDIM + 1];
static seq_t S   [MAXDIM + 1][MAXDIM + 1];
static seq_t T   [MAXDIM + 1][MAXDIM + 1];
static seq_t binom[MAXDIM + 1][MAXDIM + 1];

/* Computes a(n) */
seq_t a(idx_t n) {
    /* --- Map index n to array coordinates (n',k') --- */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) ++s;          /* find block */
    idx_t tri = s * (s + 1) / 2;
    idx_t offset = n - tri;
    int nprime = (int)offset;        /* 0 <= nprime <= s <= MAXDIM */
    int kprime = (int)(s - offset);   /* 0 <= kprime <= s <= MAXDIM */

    /* --- Prepare tables up to nprime and kprime --- */
    int maxN = nprime;
    int maxK = kprime;

    /* Powers: powv[j][i] = j^i   (j>=1, i>=1) */
    for (int j = 1; j <= maxK; ++j)
        powv[j][0] = (seq_t)1;
    for (int i = 1; i <= maxN; ++i) {
        for (int j = 1; j <= maxK; ++j)
            powv[j][i] = powv[j][i - 1] * (seq_t)j;
    }

    /* Sums of powers: S[i][k] = Σ_{j=1..k} j^i  */
    for (int i = 0; i <= maxN; ++i)
        S[i][0] = (seq_t)0;
    for (int i = 1; i <= maxN; ++i)
        for (int k = 1; k <= maxK; ++k)
            S[i][k] = S[i][k - 1] + powv[k][i];

    /* Binomial coefficients: binom[n][k] for 0 <= n <= maxN */
    for (int n = 0; n <= maxN; ++n) {
        binom[n][0] = (seq_t)1;
        binom[n][n] = (seq_t)1;
    }
    for (int n = 1; n <= maxN; ++n)
        for (int k = 1; k < n; ++k)
            binom[n][k] = binom[n - 1][k - 1] + binom[n - 1][k];

    /* Triangle values T[n][k], base case n=0 */
    for (int k = 0; k <= maxK; ++k)
        T[0][k] = (seq_t)1;

    /* Compute T for 1 <= n <= maxN */
    for (int n = 1; n <= maxN; ++n) {
        for (int k = 0; k <= maxK; ++k) {
            seq_t sum = (seq_t)0;
            for (int i = 1; i <= n; ++i) {
                seq_t Si   = S[i][k];
                seq_t bin  = binom[n - 1][i - 1];
                seq_t prev = T[n - i][k];
                seq_t prod = Si * bin;
                prod *= prev;
                sum += prod;
            }
            T[n][k] = sum;
        }
    }

    return T[maxN][maxK];
}
