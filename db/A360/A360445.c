/*
 * A360445  a(n) = Sum_{k=1..n} A178244(k-1).
 * https://oeis.org/A360445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=0}^{n-1} C(bits(m), popcount(m)) bits(m) is the
 *            length of the binary representation of m, taken to be 1 for m =
 *            0. popcount(m) is the number of 1-bits in m
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   _bitlen, _popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_bitlen.h"
#include "../utils/_popcount.h"

/* Computes the binomial coefficient C(n, k).  n and k are small (<= 64 for n <= 10000). */
static seq_t _choose(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - (k - i));
        res = res / i;
    }
    return res;
}

/* MAIN function required by the build. */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t m = 0; m < n; ++m) {
        idx_t len  = _bitlen(m);
        idx_t ones = _popcount(m);
        sum += _choose(len, ones);
    }
    return sum;
}
