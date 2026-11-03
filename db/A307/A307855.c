/*
 * A307855  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of 1/sqrt(1 - 2*x + (1-4*k)*x^2).
 * https://oeis.org/A307855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 86, u32 n <= 223, u64 n <= 585,
 *            u128 n <= 1640, big any n
 * Validated: standard, pass, 82 terms through n = 9796 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A_row_k(idx_t row, idx_t k)
{
    if (row <= 1)   /* A(0,k) = A(1,k) = 1 */
        return 1;

#if !defined(__cplusplus)   /* C implementation – use 128‑bit intermediate to avoid overflow */
    seq_t a0 = 1, a1 = 1;
    for (idx_t i = 2; i <= row; ++i)
    {
        /* term1 = (2*i-1) * a1  (use 128‑bit) */
        unsigned __int128 t1 = (unsigned __int128)(2 * i - 1) * a1;

        /* mult = (i-1) * a0  (use 128‑bit) */
        unsigned __int128 mult = (unsigned __int128)(i - 1) * a0;

        /* numerator = term1 - (1-4k)*(i-1)*a0 */
        if (k == 0) {               /* (1-4k) == 1  */
            t1 -= mult;
        } else {                    /* (1-4k) == -(4k-1)  */
            unsigned __int128 factor = (unsigned __int128)(4 * k - 1) * mult;
            t1 += factor;
        }

        /* a_i = numerator / i */
        seq_t ai = (seq_t)(t1 / i);
        a0 = a1;
        a1 = ai;
    }
    return a1;
#else   /* C++ implementation – use seq_t arithmetic (big‑int support) */
    seq_t a0 = 1, a1 = 1;
    for (idx_t i = 2; i <= row; ++i)
    {
        seq_t t1 = (seq_t)(2 * i - 1) * a1;
        seq_t mult = (seq_t)(i - 1) * a0;
        if (k == 0) {
            t1 -= mult;
        } else {
            t1 += (seq_t)(4 * k - 1) * mult;
        }
        seq_t ai = t1 / i;
        a0 = a1;
        a1 = ai;
    }
    return a1;
#endif
}
/* A307855: a(n) is the entry A(n,k) of the square array read by antidiagonals.
   A(n,k) is the coefficient of x^n in (1 + x + k*x^2)^n.
   Recurrence:
        n*A(n,k) = (2n-1)*A(n-1,k) - (1-4k)*(n-1)*A(n-2,k)
   Initial values:  A(0,k)=1,  A(1,k)=1. */

seq_t a(idx_t n)
{
    /* Locate the antidiagonal m that contains index n.
       The count of terms up to diagonal m-1 is m*(m+1)/2. */
    idx_t m = 0;
    idx_t terms_before = 0;
    while (terms_before + (m + 1) <= n)
    {
        terms_before += m + 1;
        ++m;
    }

    /* Coordinates within the diagonal: row = n - terms_before,
       column k = m - row. */
    idx_t row = n - terms_before;
    idx_t k   = m - row;

    return A_row_k(row, k);
}
