/*
 * A324010  The sum of squares of the number of common points in all pairs of
 *          lattice paths from (0,0) to (x,y), for x >= 0, y >= 0 (the
 *          unnormalized second moment). The table is read by antidiagonals.
 * https://oeis.org/A324010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (x+y+1)*C(x+y+2,x+1)*C(x+y,x) - C(2x+2y+2,2x+1)/2 * where
 *            (x,y) is obtained by reading the sequence in antidiagonal order.
 *            * See A324010 for details
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* binomial coefficient C(n,k) computed multiplicatively.            */
/* ------------------------------------------------------------------ */
static inline seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;           /* exact division at each step */
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Return the sequence value a(n).                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find diagonal d such that T(d)=d*(d+1)/2 <= n < T(d+1).       */
    idx_t d = 0;
    unsigned __int128 sum = 0;   /* sum of lengths of completed diagonals */

    /* Incrementally find d; O(sqrt(n)) at most.                         */
    while (1) {
        unsigned __int128 next = sum + (unsigned __int128)(d + 1);
        if (next > n) break;
        sum = next;
        ++d;
    }

    /* Offset within diagonal d */
    idx_t k = n - (idx_t)sum;    /* 0 <= k <= d */
    idx_t x = d - k;
    idx_t y = k;

    /* Compute the two parts of the formula */
    seq_t part1 = binom_multiplicative(x + y + 2, x + 1);
    seq_t part2 = binom_multiplicative(x + y, x);
    seq_t prod = part1 * part2;
    prod = (seq_t)(x + y + 1) * prod;

    seq_t part3 = binom_multiplicative(2 * x + 2 * y + 2, 2 * x + 1);
    part3 = part3 / 2;          /* division by 2 is exact */

    return prod - part3;
}
