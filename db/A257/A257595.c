/*
 * A257595  Expansion of x^3*(1+x+2*x^2+3*x^3+3*x^4+x^5)/(1-x^2-x^3)^3.
 * https://oeis.org/A257595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = N_n + 3*a(n-2) + 3*a(n-3) - 3*a(n-4) - 6*a(n-5) -
 *            2*a(n-6) + 3*a(n-7) + 3*a(n-8) + a(n-9) where N_n = 1,1,2,3,3,1
 *            for n = 3,4,5,6,7,8 respectively, and N_n = 0 for all other n.
 *            This follows from the generating function
 *            x^3*(1+x+2*x^2+3*x^3+3*x^4+x^5)/(1‑x^2‑x^3)^3
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 58, u64 n <= 132,
 *            u128 n <= 284, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* a(0)=a(1)=a(2)=0 */

    /* N_n: 0 up to n<3, then 1,1,2,3,3,1 for n=3..8, 0 thereafter. */
    static const seq_t N[9] =
        {0,0,0,1,1,2,3,3,1};

    seq_t *A = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!A) exit(1);                         /* memory allocation failed */

    /* Initialize first three terms */
    A[0] = A[1] = A[2] = 0;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t res = 0;

        /* add numerator term if present */
        if (i <= 8) res += N[i];

        /* recurrence coming from (1‑3x^2‑3x^3+3x^4+6x^5+2x^6‑3x^7‑3x^8‑x^9) */
        if (i >= 2) res += 3 * A[i - 2];
        if (i >= 3) res += 3 * A[i - 3];
        if (i >= 4) res -= 3 * A[i - 4];
        if (i >= 5) res -= 6 * A[i - 5];
        if (i >= 6) res -= 2 * A[i - 6];
        if (i >= 7) res += 3 * A[i - 7];
        if (i >= 8) res += 3 * A[i - 8];
        if (i >= 9) res += A[i - 9];

        A[i] = res;
    }

    seq_t val = A[n];
    free(A);
    return val;
}
