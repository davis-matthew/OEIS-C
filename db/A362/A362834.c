/*
 * A362834  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = (-1)^n * n! * Sum_{j=0..floor(n/2)} k^j
 *          * Stirling1(n-j,j)/(n-j)!.
 * https://oeis.org/A362834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) where T(n,k) = (-1)^n * n! * sum_{j=0..⌊n/2⌋} k^j
 *            * Stirling1(n-j,j)/(n-j)!, and the pair (n,k) is obtained from
 *            the antidiagonal ordering: d = ⌊(-1+√(1+8*n))/2⌋, n0 = n -
 *            d*(d+1)/2, k0 = d - n0
 * Range:     u8 n <= 32, u16 n <= 62, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

static const idx_t MAX_N = 250; /* safely covers up to n≈10000 */

static seq_t *stirling = NULL;        /* signed Stirling numbers of the first kind:
                                        stirling[n][k]  for 0 <= k <= n <= MAX_N */
static seq_t *fact     = NULL;        /* factorial[0..MAX_N] */
static int initialized = 0;

/* Simple factorial and Stirling precomputation */
static void prepare(void)
{
    if (initialized) return;
    fact = (seq_t*)malloc(sizeof(seq_t) * (MAX_N + 1));
    stirling = (seq_t*)malloc(sizeof(seq_t) * (MAX_N + 1) * (MAX_N + 1));

    /* factorials */
    fact[0] = 1;
    for (idx_t i = 1; i <= MAX_N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* signed Stirling numbers of the first kind:
       s(0,0)=1, s(n,0)=0 (n>0), s(0,k)=0 (k>0) */
    for (idx_t n = 0; n <= MAX_N; ++n)
        for (idx_t k = 0; k <= MAX_N; ++k)
            stirling[n * (MAX_N + 1) + k] = 0;
    stirling[0 * (MAX_N + 1) + 0] = 1;
    for (idx_t n = 1; n <= MAX_N; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term1 = stirling[(n - 1) * (MAX_N + 1) + (k - 1)];
            seq_t term2 = stirling[(n - 1) * (MAX_N + 1) + k];
            stirling[n * (MAX_N + 1) + k] = term1 - (seq_t)(n - 1) * term2;
        }
    }
    initialized = 1;
}

/* Compute T(n,k) using the formula. */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t sum = 0;
    idx_t maxj = n >> 1; /* floor(n/2) */
    for (idx_t j = 0; j <= maxj; ++j)
    {
        seq_t term = fact[n] / fact[n - j];           /* (n!)/(n-j)! */
        /* k^j */
        seq_t powk = 1;
        for (idx_t t = 0; t < j; ++t)
            powk *= (seq_t)k;
        term *= powk;
        term *= stirling[(n - j) * (MAX_N + 1) + j];
        sum += term;
    }
    if (n & 1)          /* if n is odd */
        sum = -sum;
    return sum;
}

/* Main entry function */
seq_t a(idx_t n)
{
    prepare();                      /* ensure tables are filled */

    /* Determine the antidiagonal (d) and the position (offset) */
    double d_double = (-1.0 + sqrt(1.0 + 8.0 * (double)n)) / 2.0;
    idx_t d = (idx_t)d_double;      /* floor because of conversion */

    idx_t start = d * (d + 1) / 2;  /* first index of diagonal d */
    idx_t offset = n - start;       /* 0 <= offset <= d */

    idx_t n0 = offset;
    idx_t k0 = d - offset;

    return compute_T(n0, k0);
}
