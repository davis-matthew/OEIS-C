/*
 * A277082  Generalized 15-gonal (or pentadecagonal) numbers: n*(13*n - 11)/2,
 *          n = 0,+1,-1,+2,-2,+3,-3, ...
 * https://oeis.org/A277082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n - 11)/2, with n = 0,+1,-1,+2,-2,+3,-3,
 * Range:     u8 n <= 11, u16 n <= 199, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence may be written as a(n) = ((-1)^n) * (n*(13*n - 11)/2) with alternating signs on n.
   For an odd index n, n corresponds to +((n+1)/2); for an even n > 0, to -((n/2)). */

seq_t a(idx_t n)
{
    /* Use signed 64‑bit arithmetic for the intermediate result.
       The value fits into int64_t for all n reasonably within tests. */
    int64_t m;
    if (n & 1ULL) {
        /* odd n : m = +( (n+1)/2 ) = n>>1 + 1  (avoids overflow on n+1) */
        m = (int64_t)(n >> 1) + 1;
    } else {
        /* even n : m = - (n/2)  */
        m = -((int64_t)(n >> 1));
    }

    int64_t val = m * (13 * m - 11) / 2;   /* division is exact */
    return (seq_t)val;
}
