/*
 * A391726  Square array read by descending antidiagonals: find the k-th odd
 *          number i that satisfies i*n = A048720(i,m) for some m, then A(n,
 *          k) = m. Here A048720 is carryless base-2 multiplication.
 * https://oeis.org/A391726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   search. Write the row as n = 2^s * n' with n' odd. Test odd i =
 *            1, 3, 5, ... in order and keep each i for which i*n' is
 *            carrylessly divisible by i (that is, A048720(i,m') = i*n'). For
 *            the k-th such i, A(n,k) = m' * 2^s, which uses A(2n,k) =
 *            2*A(n,k). Terms are read by descending antidiagonals
 * Range:     u8 n <= 278, u16 n <= 5923, u32 n <= 10440, u64 n <= 10440,
 *            u128 n <= 10440, big any n
 * Validated: standard, pass, 107 terms through n = 9619 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned gf2_bitlen(seq_t x)
{
    unsigned b = 0;
    while (x != 0) { b++; x >>= 1; }
    return b;
}

/* carryless divide D by v; returns true if exact, quotient in *q */
static bool gf2_exact_div(seq_t D, seq_t v, unsigned vl, seq_t *q)
{
    seq_t quo = 0;
    unsigned dl = gf2_bitlen(D);
    int pos;
    for (pos = (int)dl - 1; pos >= (int)vl - 1; pos--) {
        if (((D >> pos) & 1) != 0) {
            D ^= (v << (pos - (int)vl + 1));
            quo |= ((seq_t)1 << (pos - (int)vl + 1));
        }
    }
    *q = quo;
    return D == 0;
}

seq_t a(idx_t idx)
{
    idx_t d = 1;
    while ((idx_t)(d * (d + 1) / 2) < idx) d++;
    idx_t j = idx - (idx_t)(d * (d - 1) / 2);   /* 1..d */
    idx_t n = j;
    idx_t k = d + 1 - j;
    int s = 0;
    while ((n & 1) == 0) { n >>= 1; s++; }
    seq_t i = 1;
    idx_t cnt = 0;
    seq_t m = 0;
    for (;;) {
        seq_t D = i * (seq_t)n;
        seq_t q;
        if (gf2_exact_div(D, i, gf2_bitlen(i), &q)) {
            cnt++;
            if (cnt == k) { m = q; break; }
        }
        i += 2;
    }
    return m << s;
}
