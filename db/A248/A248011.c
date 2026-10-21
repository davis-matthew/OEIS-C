/*
 * A248011  Table T(n,k), n>=1, k>=1, read by antidiagonals: T(n,k) = number
 *          of equivalence classes of ways of placing three 1 X 1 tiles in an
 *          n X k rectangle under all symmetry operations of the rectangle.
 * https://oeis.org/A248011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = T(i,j) where T(n,k) = (4*k^3*n^3 - 12*k^2*n^2 + 2*k^3 +
 *            6*k^2*n + 6*k*n^2 + 2*n^3 - 12*k^2 + 11*k*n - 12*n^2 + 4*k + 4*n
 *            - 3 - (2*k^3 + 6*k^2*n - 12*k^2 + 3*k*n + 4*k - 3)*(-1)^n -
 *            (6*k*n^2 + 2*n^3 + 3*k*n - 12*n^2 + 4*n - 3)*(-1)^k + (3*k*n -
 *            3)*(-1)^k*(-1)^n) / 96 (T read by antidiagonal...
 * Range:     u8 n <= 31, u16 n <= 219, u32 n <= 9246, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square root for __int128 */
static inline __int128 isqrt128(__int128 x) {
    long double d = (long double)x;
    unsigned long long r = sqrtl(d);
    __int128 n = r;
    while ((n + 1) * (n + 1) <= x) ++n;
    while (n * n > x) --n;
    return n;
}

/* Convert 1‑based sequence index to (n,k) and evaluate T(n,k). */
seq_t a(idx_t idx) {
    /* Find the diagonal (sum s = n+k) containing the index */
    __int128 i128 = (__int128)idx;
    __int128 discriminant = 1 + 8 * i128;
    __int128 sqrt_d = isqrt128(discriminant);          /* floor(sqrt(1+8i)) */
    __int128 s = (1 + sqrt_d) / 2;                     /* floor((1+sqrt)/2) */
    while ((s - 1) * s / 2 < i128) ++s;                /* ensure s correct */

    /* Position within the diagonal */
    __int128 before = (s - 2) * (s - 1) / 2;           /* elements before this diag */
    __int128 pos = i128 - before - 1;                  /* 0‑based position in diag */

    int64_t N = (int64_t)(pos + 1);    /* row number n */
    int64_t K = (int64_t)(s - N);      /* column number k */

    /* Helper: cast to __int128 for safe arithmetic */
    __int128 n = N;
    __int128 k = K;

    /* Signs for (-1)^n and (-1)^k */
    __int128 sign_n = (N & 1) ? -1 : 1;
    __int128 sign_k = (K & 1) ? -1 : 1;

    /* Compute numerator of the formula */
    __int128 term_base =
        4*k*k*k*n*n*n
      - 12*k*k*n*n
      + 2*k*k*k
      + 6*k*k*n
      + 6*k*n*n
      + 2*n*n*n
      - 12*k*k
      + 11*k*n
      - 12*n*n
      + 4*k
      + 4*n
      - 3;

    __int128 term1 =
        2*k*k*k
      + 6*k*k*n
      - 12*k*k
      + 3*k*n
      + 4*k
      - 3;

    __int128 term2 =
        6*k*n*n
      + 2*n*n*n
      + 3*k*n
      - 12*n*n
      + 4*n
      - 3;

    __int128 term3 = 3*k*n - 3;

    __int128 numerator =
        term_base
      - term1*sign_n
      - term2*sign_k
      + term3*sign_k*sign_n;

    /* Division by 96 (exact integer division) */
    seq_t result = (seq_t)(numerator / 96);
    return result;
}
