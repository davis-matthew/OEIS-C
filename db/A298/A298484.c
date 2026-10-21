/*
 * A298484  Square array A(n,k), n >= 1, k >= 1, read by antidiagonals, where
 *          A(n,k) is defined by the following: A(1,k) = k and A(n,k) =
 *          A(n-1,k)*(A(n-1,k)+1) for n > 1.
 * https://oeis.org/A298484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * A(1,k) = k * A(n,k) = A(n-1,k) * (A(n-1,k) + 1) for n > 1 *
 *            a(n) is obtained by reading the array by antidiagonals. * For n
 *            >= 1 the n-th term is A(i,j) where (i,j) are found by
 *            antidiagonal * enumeration (i+j constant, increasing sum)
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 26, u64 n <= 34, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for non‑positive index */

    /* Find the antidiagonal index s such that
     *   (s-1)*s/2  >= n  >  (s-2)*(s-1)/2
     */
    idx_t low = 2;
    idx_t high = 2;
    while (((unsigned __int128)(high - 1) * high) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = ((unsigned __int128)(mid - 1) * mid) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                         /* current diagonal index */

    /* Number of elements before this diagonal */
    unsigned __int128 prev128 = ((unsigned __int128)(s - 2) * (s - 1)) / 2;
    idx_t prev = (idx_t)prev128;           /* safe cast: prev < n */

    idx_t offset = n - prev - 1;           /* 0‑based offset on the diagonal */
    idx_t i = offset + 1;                  /* row number            */
    idx_t j = s - i;                       /* column number         */

    seq_t x = (seq_t)j;
    for (idx_t k = 1; k < i; ++k) {
        x = x * (x + 1);
    }
    return x;
}
