/*
 * A361261  Array of Ramsey core number rc(s,t) read by antidiagonals.
 * https://oeis.org/A361261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = rc(s,t) where a(n) is the sequence A361261. rc(s,t) =
 *            ceil( s + t - 3/2 + sqrt( 2*(s-1)*(t-1) + 9/4 ) ) The terms are
 *            read by antidiagonals of the rc–matrix with offset 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for 64‑bit values.
   Uses binary search with __int128 to avoid overflow. */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0, high = 1ULL << 32;   /* sqrt(UINT64_MAX) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Convert n to the pair (s,t) given by reading rc(s,t) by antidiagonals. */
static void antidiagonal_coords(idx_t n, idx_t *s, idx_t *t)
{
    /* n is 1‑based */
    idx_t offset = n - 1;                 /* zero‑based */
    idx_t k = (floor_sqrt_u64(8 * offset + 1) - 1) >> 1; /* diagonal index */
    idx_t start = k * (k + 1) / 2;        /* first index on diagonal */
    idx_t local = offset - start;         /* position on diagonal */
    *s = local + 1;                       /* s ≥ 1 */
    *t = (k + 1) - local;                 /* t ≥ 1 */
}

/* Main function requested by the exercise. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* sequence starts at n=1 */

    idx_t s, t;
    antidiagonal_coords(n, &s, &t);

    idx_t u = s - 1;                      /* u = s-1 */
    idx_t v = t - 1;                      /* v = t-1 */
    idx_t w = 8 * u * v + 9;              /* 8*u*v + 9  (fits in 64‑bit) */
    idx_t f = floor_sqrt_u64(w);          /* floor sqrt of w */

    idx_t N = 2 * (s + t) - 3;            /* 2*(s+t)-3 */

    idx_t temp = N + f;                   /* N + floor(sqrt(w)) */
    idx_t rc;                             /* value we compute in 64‑bit */

    if ((unsigned __int128)f * f == w) {  /* w is a perfect square */
        rc = temp / 2 + (temp & 1);       /* ceil((N+sqrt(w))/2) */
    } else {                              /* w not a square */
        rc = temp / 2 + 1;                /* ceil((N+sqrt(w))/2) */
    }

    return (seq_t)rc;
}
