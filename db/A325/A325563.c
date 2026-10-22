/*
 * A325563  a(1) = 1; for n > 1, a(n) is the largest proper divisor d of n
 *          such that A048720(d,k) = n for some k.
 * https://oeis.org/A325563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest proper divisor d of n such that the binary
 *            polynomial of d divides the binary polynomial of n over GF(2)
 * Range:     u8 n <= 510, u16 n <= 7722, u32 n <= 7722, u64 n <= 7722,
 *            u128 n <= 7722, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* bit length of a seq_t value */
static int bitlen_seq_t(seq_t x)
{
    int len = 0;
    while (x) { x >>= 1; ++len; }
    return len;
}

/* test whether the binary polynomial of d divides that of n over GF(2) */
static bool poly_divides(seq_t n, seq_t d)
{
    /* d > 0 assumed */
    seq_t rem = n;
    int deg_d = bitlen_seq_t(d) - 1;
    if (deg_d < 0) return false;   /* d == 0 impossible */

    while (1) {
        int deg_rem = bitlen_seq_t(rem) - 1;
        if (deg_rem < deg_d) break;
        unsigned int shift = deg_rem - deg_d;
        rem ^= (d << shift);
    }
    return rem == 0;
}

/* integer square root floor of an idx_t value (avoid external libs) */
static idx_t int_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t left = 1, right = n / 2, ans = 1;
    while (left <= right) {
        idx_t mid = left + (right - left) / 2;
        if (mid <= n / mid) { ans = mid; left = mid + 1; }
        else right = mid - 1;
    }
    return ans;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t N = (seq_t)n;
    seq_t best = (seq_t)1;

    idx_t sq = int_sqrt(n);
    for (idx_t i = 1; i <= sq; ++i) {
        if (n % i != 0) continue;
        seq_t d1 = (seq_t)i;
        seq_t d2 = (seq_t)(n / i);

        if (d1 != N && poly_divides(N, d1) && d1 > best) best = d1;

        if (d2 != N && d2 != d1 && poly_divides(N, d2) && d2 > best) best = d2;
    }

    return best;
}
