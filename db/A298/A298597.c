/*
 * A298597  Number T(n,k) of times the value k appears on the parking
 *          functions of length n and such that if we replace that value k
 *          with k+1 we don't get a parking function.
 * https://oeis.org/A298597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Flat sequence of triangle T(n,k)
   where T(n,k) = n*binomial(n-1,k-1)*k^(k-2)*(n+1-k)^(n-1-k)   */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* Compute one triangle entry T(n,k) */
static seq_t
triangle_T(idx_t n, idx_t k)
{
    /* n * binomial(n-1,k-1) */
    seq_t result = (seq_t)n * binomial(n - 1, k - 1);

    /* k^(k-2) – if k<2, treat as 1 */
    if (k >= 2)
        result *= seq_pow((seq_t)k, k - 2);

    /* (n+1-k)^(n-1-k) – if n-1-k<0, treat as 1 */
    if (n - 1 >= k)
        result *= seq_pow((seq_t)(n + 1 - k), n - 1 - k);

    return result;
}

/* Main function: a(n) for any n >= 1.  The sequence is the flattened
   triangle row‑by‑row (row m has m terms). */
seq_t
a(idx_t n)
{
    /* Find the row m such that m*(m+1)/2 >= n */
    idx_t low  = 1;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t row = low;                     /* m */
    idx_t prev_tri = (row - 1) * row / 2;/* (m-1)*m/2 */
    idx_t col = n - prev_tri;            /* k = 1 .. m */

    return triangle_T(row, col);
}
