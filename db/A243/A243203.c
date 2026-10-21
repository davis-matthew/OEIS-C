/*
 * A243203  Triangle read by rows: T(n, k) = k * n^(n - k - 1) * n! / (n - k)!
 *          for n >= 1, T(0, 0) = 1.
 * https://oeis.org/A243203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle read by rows, where
 *            T(n,k) = k * n^(n-k) * ∏_{i=1}^{k-1} (n-i) (n ≥ 0, 0 ≤ k ≤ n),
 *            with the conventions T(0,0)=1 and T(n,0)=0 for n>0. The index n
 *            determines the row r by r(r+1)/2 ≤ n < (r+1)(r+2)/2, and the
 *            column k = n − r(r+1)/2. Then a(n)=T(r,k)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 66, u64 n <= 153,
 *            u128 n <= 406, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"   /* provides seq_t pow_seq(seq_t, int) */

seq_t a(idx_t n)
{
    /* Handle the special first element. */
    if (n == 0) return (seq_t)1;

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) r++;

    /* Column index k within this row. */
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;

    /* For n>0, column 0 gives value 0. */
    if (k == 0) return (seq_t)0;

    /* If k>0, compute the product part ∏_{i=1}^{k-1} (r-i). */
    seq_t prod = 1;
    for (idx_t i = 1; i < k; ++i)
        prod *= (seq_t)(r - i);

    /* Compute r^(r-k) using the provided power routine. */
    seq_t pow = pow_seq((seq_t)r, (int)(r - k));

    /* Final value: k * r^(r-k) * ∏_{i=1}^{k-1} (r-i). */
    return (seq_t)k * pow * prod;
}
