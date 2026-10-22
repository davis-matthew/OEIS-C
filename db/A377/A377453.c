/*
 * A377453  E.g.f. satisfies A(x) = 1/(1 - A(x) * (exp(x) - 1))^3.
 * https://oeis.org/A377453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Sum_{k=0..n} (4k+2)!/(3k+3)! * Stirling2(n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 3 * Sum_{k=1..n} f(k) * S(n,k)  for n>0 */
/* where f(k) = (4k+2)!/(3k+3)!  and S(n,k) are Stirling numbers of the 2nd kind */

#include <stdlib.h>

/* Compute A377453 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Allocate array for Stirling numbers of the second kind S(n,k) */
    seq_t *S = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!S) return (seq_t)0;          /* allocation failure, return 0 */
    S[0] = (seq_t)1;                  /* S(0,0)=1 */

    /* Compute Stirling numbers S(i,j) via recurrence:
       S(i,j) = j * S(i-1,j) + S(i-1,j-1)  (with S(i,j)=0 if j>i) */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j > 0; --j) {
            seq_t tmp = S[j];               /* old S[i-1][j] */
            S[j] = tmp * (seq_t)j + S[j - 1];/* j * S[i-1][j] + S[i-1][j-1] */
        }
        S[0] = (seq_t)0;                      /* S(i,0)=0 for i>=1 */
    }

    /* Sum_{k=1..n} f(k) * S(n,k) with f(k) = (4k+2)!/(3k+3)!  */
    seq_t sum = (seq_t)0;
    seq_t f   = (seq_t)1; /* f(1) = 1 */
    sum += f * S[1];      /* add contribution for k = 1 */

    for (idx_t k = 1; k < n; ++k) {
        /* Compute next f(k+1) using the recurrence:
           f(k+1) = f(k) * prod_{i=4k+3}^{4k+6} i
                                / prod_{i=3k+4}^{3k+6} i  */
        unsigned __int128 num = 1;
        unsigned __int128 den = 1;
        for (idx_t i = 4 * k + 3; i <= 4 * k + 6; ++i) num *= i;
        for (idx_t i = 3 * k + 4; i <= 3 * k + 6; ++i) den *= i;

        /* f may be as large as 64‑bit; use __int128 for intermediate */
        unsigned __int128 prod = (unsigned __int128)f * num;
        prod /= den;                       /* exact division */
        f = (seq_t)prod;

        sum += f * S[k + 1];
    }

    seq_t result = sum * (seq_t)3;
    free(S);
    return result;
}
