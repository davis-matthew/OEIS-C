/*
 * A257092  Square array read by antidiagonals: Nimsum function for
 *          "Take-or-Break" Nim where a legal move is defined as: 1) Remove a
 *          nonzero number of counters from any pile up to the size of the
 *          selected pile OR 2) Split any pile of size greater than one into
 *          two nonzero piles (removing no counters from th...
 * https://oeis.org/A257092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look, easy
 * Formula:   a(n) = Flip(Flip(i) XOR Flip(j)) where (i,j) are the coordinates
 *            read by antidiagonals of the square array given by NSum(x,y) =
 *            Flip(Flip(x) XOR Flip(y))
 * Range:     u8 n <= 9971, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   flip
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/flip.h"

/* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2.
   Binary search in [0, n] with 128‑bit arithmetic to avoid overflow. */
static inline idx_t diag_from_n(idx_t n) {
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* Locate (i,j) from antidiagonal reading */
    idx_t d = diag_from_n(n);
    __int128 start128 = (__int128)d * (d + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t j = n - start;
    idx_t i = d - j;

    seq_t Fi = flip((seq_t)i);
    seq_t Fj = flip((seq_t)j);
    seq_t x = Fi ^ Fj;
    return flip(x);
}
