/*
 * A362837  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = (-1)^n * n! * Sum_{j=0..floor(n/2)}
 *          k^(n-j) * Stirling1(n-j,j)/(n-j)!.
 * https://oeis.org/A362837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) with i = n - s*(s+1)/2, j = s - i, where s =
 *            floor((sqrt(8n+1)-1)/2)
 * Range:     u8 n <= 31, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* T(n,k) = n! * Σ_{j=0..⌊n/2⌋} k^{n-j} * Stirling1(n-j,j)/(n-j)!  (unsigned Stirling numbers) */

#include <stdbool.h>

#define NMAX 200

static seq_t C[NMAX+1][NMAX+1];      /* unsigned Stirling numbers of the first kind */
static bool initialized = false;

/* exponentiation by repeated multiplication (base^exp) */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res *= base;
    }
    return res;
}

/* compute T(n,k) as defined above */
static seq_t compute_T(idx_t n, idx_t k) {
    if (n == 0) return (seq_t)1;

    seq_t sum  = 0;
    idx_t maxj = n >> 1;          /* floor(n/2) */
    seq_t ratio = 1;              /* n!/(n-j)! for current j */

    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t m = n - j;                              /* m = n-j */
        seq_t powk = pow_seq((seq_t)k, m);            /* k^{n-j} */
        seq_t term = powk * C[m][j];                  /* k^{n-j} * Stirling1(m,j) */
        term *= ratio;                                /* multiply by n!/(n-j)! */
        sum += term;

        if (j < maxj) {
            ratio *= (seq_t)(n - j);   /* prepare ratio for next j */
        }
    }
    return sum;
}

/* precompute Stirling numbers of the first kind */
static void init(void) {
    if (initialized) return;
    initialized = true;

    C[0][0] = 1;
    for (idx_t n = 1; n <= NMAX; ++n) {
        C[n][0] = 0;
        for (idx_t k = 1; k <= n; ++k) {
            C[n][k] = C[n-1][k-1] + (seq_t)(n-1) * C[n-1][k];
        }
    }
}

/* main function requested by OEIS */
seq_t a(idx_t n) {
    init();                          /* make sure precomputation is done */

    /* find diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ((s+1)*(s+2)/2 <= n) ++s;

    idx_t start = s*(s+1)/2;
    idx_t k = n - start;        /* offset within this diagonal */
    idx_t i = k;                /* row index in T */
    idx_t j = s - k;            /* column index in T */

    return compute_T(i, j);
}
