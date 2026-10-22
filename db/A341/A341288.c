/*
 * A341288  Square array T(n, k), read by antidiagonals, n, k >= 0; T(n, k) =
 *          XOR_{u in B(n), v in B(k)} 2^(u XOR v) where XOR denotes the
 *          bitwise XOR operator and B(n) gives the exponents in expression
 *          for n as a sum of powers of 2.
 * https://oeis.org/A341288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look, base
 * Formula:   a(n) = T(i, j) where d = floor((sqrt(8n+1)-1)/2), i = n -
 *            d(d+1)/2, j = d - i. T(i, j) = XOR_{u ∈ B(i), v ∈ B(j)} 2^(u XOR
 *            v), B(x) = set of bit positions of 1 in binary representation of
 *            x
 * Range:     u8 n <= 9140, u16 n <= 9140, u32 n <= 9140, u64 n <= 9140,
 *            u128 n <= 9140, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t T(idx_t a, idx_t b)
{
    seq_t res = 0;
    /* iterate over set bits of a */
    for (idx_t u_bits = a; u_bits; u_bits &= u_bits - 1) {
        idx_t u = __builtin_ctzll(u_bits);
        /* iterate over set bits of b */
        for (idx_t v_bits = b; v_bits; v_bits &= v_bits - 1) {
            idx_t v = __builtin_ctzll(v_bits);
            seq_t bit = (seq_t)1 << (u ^ v);
            res ^= bit;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find diagonal d: smallest d such that (d+1)(d+2)/2 > n */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        d++;

    /* compute coordinates on this diagonal */
    idx_t i = n - d * (d + 1) / 2;
    idx_t j = d - i;

    return T(i, j);
}
