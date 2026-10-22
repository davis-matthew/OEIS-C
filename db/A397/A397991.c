/*
 * A397991  Array read by ascending antidiagonals: A(n,k) = floor(1+k*n/(k-1))
 *          with k >= 2.
 * https://oeis.org/A397991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = floor(1 + k*n/(k-1)), where (n,k) are the parameters of
 *            the * array element found by reading the table of A(n,k) = ⌊1 +
 *            k*n/(k-1)⌋ * (k ≥ 2) along ascending antidiagonals. For a given
 *            index i, let * s be the smallest integer such that (s-1)*s/2 >
 *            i. Then * offset = i - (s-2)*(s-1)/2 * k =...
 * Range:     u8 n <= 8255, u16 n <= 9697, u32 n <= 9697, u64 n <= 9697,
 *            u128 n <= 9697, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Integer square‑root: floor(sqrt(x)) for idx_t (64‑bit unsigned). */
static idx_t isqrt_idx_t(idx_t x)
{
    if (x < 2) return x;
    idx_t low  = 1;
    idx_t high = x >> 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= x / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t i)
{
    /* Find the antidiagonal sum s.  (s-1)*s/2 > i  >= (s-2)*(s-1)/2. */
    idx_t t = isqrt_idx_t(1 + 8 * i);          /* floor(sqrt(1+8i))      */
    idx_t s = (1 + t) / 2 + 1;                 /* corresponds to the sum n+k */

    /* Elements before this diagonal. */
    idx_t prev = (s - 2) * (s - 1) / 2;
    idx_t offset = i - prev;                   /* 0 … s-2 */

    /* Parameters for A(n,k). */
    idx_t k = offset + 2;
    idx_t n = s - 2 - offset;

    /* Compute floor(1 + k*n/(k-1)) using seq_t arithmetic. */
    seq_t sk = (seq_t)k;
    seq_t sn = (seq_t)n;
    return (seq_t)1 + (sk * sn) / (sk - 1ull);
}
