/*
 * A379343  Square array read by upward antidiagonals: T(n,k) = (2*(k+n-1)^2 +
 *          2*k - 2*n + 3 + (-2*k - 2*n + 3)*(-1)^n - (-1)^k + (-2*k - 2*n +
 *          1)*(-1)^(k+n))/4.
 * https://oeis.org/A379343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = (2*(k+n-1)^2 + 2*k - 2*n + 3 + (-2*k - 2*n + 3)*(-1)^n
 *            - (-1)^k + (-2*k - 2*n + 1)*(-1)^(k+n)) / 4
 * Range:     u8 n <= 234, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the n-th term of OEIS A379343, read by
   upward anti-diagonals of the square array T. */

seq_t a(idx_t n)
{
    /* --- locate (row, col) in T by anti-diagonal enumeration --- */
    idx_t cum = 0;      /* elements counted so far */
    idx_t s = 2;        /* current anti-diagonal (sum of indices) */

    /* Find the first diagonal s such that cum + (s-1) >= n   */
    while (cum + (s - 1) < n) {
        cum += s - 1;
        ++s;
    }

    /* offset inside this diagonal (0‑based) */
    idx_t offset = n - cum - 1;

    /* coordinates in T: row = i, column = k */
    idx_t i = s - 1 - offset;     /* row index   (1‑based) */
    idx_t k = s - i;              /* column index(1‑based) */

    /* --- compute T(i,k) using the given formula --- */
    /* Use signed __int128 for intermediate arithmetic to avoid
       overflow and handle negative terms safely.   */
    __int128 ii = i;
    __int128 kk = k;
    __int128 kpik = kk + ii - 1;               /* k + n - 1   */

    __int128 term1 = 2 * kpik * kpik;               /* 2*(k+n-1)^2 */
    __int128 term2 = 2 * kk - 2 * ii + 3;           /* 2*k - 2*n + 3 */

    __int128 sign_n = (ii & 1) ? -1 : 1;            /* (-1)^n      */
    __int128 term3 = (-2 * kk - 2 * ii + 3) * sign_n;

    __int128 sign_k = (kk & 1) ? -1 : 1;            /* (-1)^k      */
    __int128 term4 = -sign_k;                      /* -(-1)^k     */

    __int128 sign_kn = ((kk + ii) & 1) ? -1 : 1;    /* (-1)^(k+n)  */
    __int128 term5 = (-2 * kk - 2 * ii + 1) * sign_kn;

    __int128 numer = term1 + term2 + term3 + term4 + term5;
    __int128 value = numer / 4;                    /* division exact */

    return (seq_t)value;
}
