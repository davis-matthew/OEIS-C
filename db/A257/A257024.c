/*
 * A257024  Number of squares in the quarter-sum representation of n.
 * https://oeis.org/A257024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each integer n, greedily subtract the largest distinct element
   from the set {k^2 , k^2+k} (k ≥ 1) that does not exceed the remaining sum.
   Count how many of the chosen elements are ordinary squares.  */
#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) using 128-bit arithmetic to avoid overflow. */
static idx_t _isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) { ans = mid; lo = mid + 1; }
        else hi = mid - 1;
    }
    return ans;
}

seq_t a(idx_t n) {
    /* Special case for n == 0 */
    if (n == 0) return (seq_t)1;

    idx_t rem = n;
    seq_t squares = 0;

    while (rem > 0) {
        /* Largest square ≤ rem */
        idx_t sq_k = _isqrt_u64(rem);
        idx_t sq_val = sq_k * sq_k;

        /* Largest quarter‑square ≤ rem : k^2 + k */
        idx_t q_val = 0;
        /* Solve k^2 + k ≤ rem  →  k = floor( (√(1+4rem) - 1)/2 ) */
        idx_t t = _isqrt_u64(1 + 4 * rem);
        idx_t k_q = (t > 0) ? (t - 1) >> 1 : 0;
        q_val = k_q * (k_q + 1);

        /* Choose the larger of the two candidates */
        if (sq_val >= q_val) {
            /* Use the square */
            rem -= sq_val;
            squares++;
        } else {
            /* Use the quarter‑square */
            rem -= q_val;
        }
    }
    return squares;
}
