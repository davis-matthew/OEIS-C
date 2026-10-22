/*
 * A342905  Array read by antidiagonals: T(n,k) = product of all distinct
 *          primes dividing n*k (n>=1, k>=1).
 * https://oeis.org/A342905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   not stated
 * Range:     u8 n <= 509, u16 n <= 9455, u32 n <= 9455, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Antidiagonal radical: a(n) = rad(i*j) where i+j minimal s with s*(s-1)/2 >= n */
#include "../utils/rad.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but guard against 0 */

    /* Find minimal integer s such that s*(s-1)/2 >= n */
    idx_t low = 1;
    idx_t high = 1;
    while ((high * (high - 1)) / 2 < n) high <<= 1;   /* exponential grow */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid - 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Number of elements before this diagonal */
    idx_t before = (s - 1) * (s - 2) / 2;
    /* Zero‑based position inside the diagonal */
    idx_t offset = n - before - 1;

    idx_t i = offset + 1;
    idx_t j = s - i;

    idx_t product = i * j;
    return rad(product);
}
