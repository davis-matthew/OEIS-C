/*
 * A275043  Number A(n,k) of set partitions of [k*n] such that within each
 *          block the numbers of elements from all residue classes modulo k
 *          are equal for k>0, A(n,0)=1; square array A(n,k), n>=0, k>=0, read
 *          by antidiagonals.
 * https://oeis.org/A275043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,j) with (i,j) obtained from the * antidiagonal
 *            mapping: a(n) = 1 when i=0 or j=0. * A(n,k) is defined
 *            recursively by * A(0,k) = 1, A(n,0) = 1 * A(n,k) = Σ_{s=1}^{n}
 *            C(n-1,s-1) · (C(n,s))^{k-1} · A(n-s,k) * where C(x,y) are
 *            binomial coefficients. * The implementation below builds a
 *            tabl...
 * Range:     u8 n <= 30, u16 n <= 40, u32 n <= 70, u64 n <= 110,
 *            u128 n <= 161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

#if defined(_WIN32) && defined(_MSC_VER)
  /* Win32 uses __int128 only in C++;  fallback for builds that
   * do not provide it.  In that case seq_t will be an unsigned
   * 128-bit type or smaller, so the operations below are fine. */
#endif

/* ------------------------------------------------------------------ */
/*  Constants and static storage                                       */
/* ------------------------------------------------------------------ */
#define MAXD 200  /* maximum index needed for i or j (>= sqrt(2*maxIndex)) */

static seq_t fact[MAXD + 1];
static seq_t dp[MAXD + 1][MAXD + 1];
static bool   done[MAXD + 1][MAXD + 1];

/* ------------------------------------------------------------------ */
/*  Helper functions                                                   */
/* ------------------------------------------------------------------ */
static void init_fact(void) {
    static bool init = false;
    if (init) return;
    init = true;
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAXD; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
}

static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    return fact[n] / (fact[k] * fact[n - k]);
}

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

static seq_t compute_A(idx_t n, idx_t k) {
    if (k == 0 || n == 0) return (seq_t)1;
    if (done[n][k]) return dp[n][k];
    seq_t total = (seq_t)0;
    for (idx_t s = 1; s <= n; ++s) {
        seq_t c1 = binom(n - 1, s - 1);
        seq_t c2 = binom(n, s);
        seq_t power = pow_seq(c2, k - 1);
        seq_t term = c1 * power;
        term *= compute_A(n - s, k);
        total += term;
    }
    dp[n][k] = total;
    done[n][k] = true;
    return total;
}

/* ------------------------------------------------------------------ */
/*  Main routine exported by the judge                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* initialise factorials and base cases only once */
    static bool init = false;
    if (!init) {
        init = true;
        init_fact();
        for (idx_t i = 0; i <= MAXD; ++i) {
            dp[0][i] = (seq_t)1;
            done[0][i] = true;
            dp[i][0] = (seq_t)1;
            done[i][0] = true;
        }
    }

    /* find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    size_t d = 0;
    while ((d + 1U) * (d + 2U) / 2U <= n) ++d;

    size_t offset = (size_t)n - d * (d + 1U) / 2U;
    idx_t i = (idx_t)offset;
    idx_t j = (idx_t)(d - offset);

    if (i == 0 || j == 0) return (seq_t)1;
    return compute_A((idx_t)i, (idx_t)j);
}
