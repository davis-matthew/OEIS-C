/*
 * A365185  G.f. satisfies A(x) = 1 + x*A(x)^5*(1 + x*A(x)).
 * https://oeis.org/A365185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+4*k+1,k) *
 *            binomial(k,n-k)/(n+4*k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Or equivalently, the generating function satisfies
        A(x) = 1 + x*A(x)^5*(1 + x*A(x)),
    giving the recurrence a(0)=1, a(n)=coeff_{n-1} A^5 + coeff_{n-2} A^6. */

#include <stdlib.h>

/* Cache for coefficients of A(x) and of its powers A^k (k=1..6). */
static seq_t *a_seq = NULL;      /* a_seq[n] = a(n)                          */
static seq_t *B[7] = {NULL};     /* B[k][n] = coeff of x^n in A^k           */
static idx_t  cap   = 0;         /* current allocation size (number of coeffs) */

/* Ensure capacity for coefficients up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (n < cap) return;                     /* already have enough space */

    /* Grow allocation to a power of two covering n. */
    idx_t newcap = cap ? cap : 1;
    while (newcap <= n) newcap <<= 1;

    a_seq = (seq_t *)realloc(a_seq, newcap * sizeof(seq_t));
    for (int k = 1; k <= 6; ++k)
        B[k] = (seq_t *)realloc(B[k], newcap * sizeof(seq_t));

    /* Initialise the very first coefficient. */
    if (cap == 0) {
        a_seq[0] = 1;
        for (int k = 1; k <= 6; ++k)
            B[k][0] = 1;                     /* A^k has constant term 1 */
    }

    /* Compute new coefficients from the old boundary up to n. */
    for (idx_t cur = (cap > 0 ? cap : 1); cur <= n; ++cur) {
        seq_t val = 0;
        if (cur >= 1) val += B[5][cur - 1];   /* coeff of x^{cur} in x*A^5   */
        if (cur >= 2) val += B[6][cur - 2];   /* coeff of x^{cur} in x^2*A^6  */
        a_seq[cur] = val;                     /* a(cur) = coeff of x^cur in A(x) */

        /* Update convolution tables for powers of A. */
        B[1][cur] = val;                      /* A^1 is just A itself */
        for (int k = 2; k <= 6; ++k) {
            seq_t sum = 0;
            for (idx_t i = 0; i <= cur; ++i)
                sum += a_seq[i] * B[k - 1][cur - i];
            B[k][cur] = sum;
        }
    }

    cap = n + 1;                              /* we now have indices [0..n] */
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return a_seq[n];
}
