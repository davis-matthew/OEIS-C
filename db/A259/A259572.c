/*
 * A259572  Reciprocity array of 0; rectangular, read by antidiagonals.
 * https://oeis.org/A259572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(i,j), where * T(m,n) = (m*n - m - n + gcd(m,n)) / 2 *
 *            and (i,j) are the unique positive integers such that n is the *
 *            position of (i,j) in the array read by antidiagonals (sum of *
 *            indices increasing, then i increasing)
 * Range:     u8 n <= 762, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the Euclidean GCD routine for seq_t */
#include "../utils/gcd.h"

/* Integer square‑root for 64‑bit unsigned values.
 * Returns ⌊√x⌋. Uses a binary search with 128‑bit products
 * to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;   /* √(2^64−1) = 2^32−1 */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    /* Step 1: find the antidiagonal index s such that
       C(s-1) < n ≤ C(s), with C(s)= (s-1)*s/2. */
    idx_t disc = 1 + 8 * n;                  /* discriminant: 1 + 8n */
    idx_t sqrt_d = isqrt_u64(disc);
    idx_t s = (1 + sqrt_d) / 2;              /* floor of the root */
    /* If C(s) < n, increment s */
    unsigned __int128 c_s = (unsigned __int128)(s - 1) * s / 2;
    if (c_s < n)
        ++s;

    /* Step 2: compute the position within this antidiagonal */
    unsigned __int128 c_prev = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t i = n - (idx_t)c_prev;             /* i = n - C(s-1) */
    idx_t j = s - i;                         /* j follows from the sum */

    /* Step 3: compute T(i,j) */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t product = ii * jj;
    seq_t g = gcd(ii, jj);
    seq_t val = (product - ii - jj + g) / 2;
    return val;
}
