/*
 * A343283  Dirichlet g.f.: Product_{k>=2} 1 / (1 - k^(-s))^(k^3).
 * https://oeis.org/A343283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all factorizations n = \prod_{k\ge 2} k^{e_k} *
 *            of \prod_{k} binom(k^3 + e_k - 1, e_k) * The program computes
 *            the Dirichlet series coefficients of * Product_{k>=2} 1/(1 -
 *            k^{-s})^(k^3) * using a recursion over the permitted integer
 *            bases. * Complexity is roughly proportional to th...
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, not used directly but standard */

static seq_t binom_seq(seq_t a, seq_t b)
{
    seq_t res = 1;
    for (seq_t i = 1; i <= b; ++i) {
        res = res * (a - b + i) / i;
    }
    return res;
}

static seq_t rec(idx_t n, idx_t max_base)
{
    if (n == 1) return 1;          /* empty factorization */
    seq_t total = 0;
    for (idx_t d = 2; d <= max_base && d <= n; ++d) {
        if (n % d != 0) continue;

        /* largest exponent e such that d^e divides n */
        idx_t e = 0;
        idx_t tmp = n;
        while (tmp % d == 0) {
            ++e;
            tmp /= d;
        }

        seq_t powd3 = (seq_t)d * (seq_t)d * (seq_t)d;
        idx_t powd_curr = d;  /* d^1 initially */

        for (idx_t eprime = 1; eprime <= e; ++eprime) {
            seq_t weight = binom_seq(powd3 + (seq_t)eprime - 1, (seq_t)eprime);
            idx_t rem = n / powd_curr;
            seq_t sub = (rem == 1) ? 1 : rec(rem, d - 1);
            total += weight * sub;

            if (eprime < e) powd_curr *= d;   /* advance to d^(eprime+1) */
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    return rec(n, n);
}
