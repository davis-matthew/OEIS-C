/*
 * A243866  Table T(n,k), n>=1, k>=1, read by antidiagonals: T(n,k) = number
 *          of equivalence classes of ways of placing one 1 X 1 tile in an n X
 *          k rectangle under all symmetry operations of the rectangle.
 * https://oeis.org/A243866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = sum_{k=1}^{n} floor((k+1)/2) * floor((n-k+2)/2) (i.e. the
 *            antidiagonal sum of the table T(i,j) = floor((i+1)/2) *
 *            floor((j+1)/2).)
 * Range:     u8 n <= 45, u16 n <= 2612, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is defined for n>=1 */

    /* Find the diagonal s such that
         (s-1)*s/2 >= n   and   (s-2)*(s-1)/2 < n       */
    idx_t low = 1, high = n + 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        seq_t cum = ((seq_t)mid * (mid - 1)) / 2;   /* safe in seq_t */
        if (cum >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t s = low;   /* diagonal number (s >= 2 for n >= 1) */

    /* Number of terms before this diagonal:
         prev = (s-2)*(s-1)/2                                     */
    seq_t prev = ((seq_t)(s - 2) * (s - 1)) / 2;
    seq_t r = (seq_t)n - prev - 1;      /* 0‑based position in diagonal */

    /* Coordinates within the diagonal:
         i = r + 1 ,   j = s - i                                  */
    idx_t i = (idx_t)(r + 1);
    idx_t j = s - i;

    /* Compute floor((i+1)/2) and floor((j+1)/2)                           */
    seq_t hi = ((seq_t)(i + 1)) / 2;
    seq_t hj = ((seq_t)(j + 1)) / 2;

    return hi * hj;
}
