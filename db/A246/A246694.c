/*
 * A246694  Triangle read by rows: T(n,k) = T(n,k-2) + 1 if n > 1 and 2 <= k
 *          <= n; T(0,0) = 1, T(1,0) = 1, T(1,1) = 2; if n > 1 is odd, then
 *          T(n,0) = T(n-1,n-2) + 1 and T(n,1) = T(n-1,n-1) + 1; if n > 1 is
 *          even, then T(n,0) = T(n-1,n-1) + 1 and T(n,1) = T(n-1,n-2) + 1.
 * https://oeis.org/A246694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) = 1 + floor(n/2)*(k even?1:0) +
 *            (floor(n/2))^2 * + floor(k/2) + (n odd && k odd? n : 0). * r is
 *            the largest integer with r(r+1)/2 ≤ n, and k = n – r(r+1)/2. *
 *            The triangle is read row‑wise, so a(0) = 1, a(1) = 1, a(2) = 2,
 *            …
 * Range:     u8 n <= 494, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer (floor).  */
static inline idx_t isqrt_uint128(__uint128_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 63;          /* start with the largest possible bit */
    while (bit > 0) {
        idx_t trial = r + bit;
        if ( (__uint128_t)trial * trial <= x )
            r = trial;
        bit >>= 1;
    }
    return r;
}

/* Triangular number r(r+1)/2, computed in 128‑bit to avoid overflow. */
static inline idx_t tri(idx_t r)
{
    return (idx_t)(((__uint128_t)r * (r + 1)) / 2);
}

/* OEIS A246694: read‑by‑rows triangle. */
seq_t a(idx_t n)
{
    /* Find r: largest r with r(r+1)/2 ≤ n. */
    idx_t r = (idx_t)((isqrt_uint128(((__uint128_t)8 * n + 1)) - 1) / 2);

    /* Correct r in case of rounding errors.  */
    while ( ( (__uint128_t)(r + 1) * (r + 2) ) / 2 <= n )
        r++;
    while ( ( (__uint128_t)r * (r + 1) ) / 2 > n )
        r--;

    idx_t k = n - tri(r);                    /* column index */

    seq_t fn    = (seq_t)(r >> 1);               /* floor(r/2) */
    seq_t term2 = ((k & 1) == 0) ? fn : 0;       /* add fn if k even */
    seq_t term3 = fn * fn;                       /* fn^2 */
    seq_t term4 = (seq_t)(k >> 1);               /* floor(k/2) */
    seq_t term5 = ((r & 1) && (k & 1)) ? r : 0; /* add r if both are odd */

    return (seq_t)1 + term2 + term3 + term4 + term5;
}
