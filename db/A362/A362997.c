/*
 * A362997  Triangle read by rows. T(n, k) = denominator([x^k] R(n, n, x)),
 *          where R(n, k, x) = Sum_{u=0..k} ( Sum_{j=0..u} x^j * binomial(u,
 *          j) * (j + 1)^n ) / (u + 1).
 * https://oeis.org/A362997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = denominator of coefficient [x^k] R(n,n,x), where integer
 *            n is the flattened index of the triangle defined in OEIS
 *            A362997. The triangular array satisfies T(n,k) = lcm(1,…,n+1) *
 *            A362996(n,k) / A362995(n,k)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 48, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* 1. recover row n_row and column k from the flattened index n */
    idx_t n_row, k_col;
    {
        unsigned __int128 target = (unsigned __int128)n * 2;
        idx_t low = 0, high = 1;
        while ((unsigned __int128)high * (high + 1) <= target)
            high <<= 1;
        while (low + 1 < high) {
            idx_t mid = low + (high - low) / 2;
            if ((unsigned __int128)mid * (mid + 1) <= target)
                low = mid;
            else
                high = mid;
        }
        n_row = low;
        unsigned __int128 base = (unsigned __int128)n_row * (n_row + 1) / 2;
        k_col = n - (idx_t)base;
    }

    /* 2. (k+1)^n_row once for the whole row */
    seq_t power = seq_pow((seq_t)(k_col + 1), (idx_t)n_row);

    /* 3. add the fractions for u = k .. n_row
       coefficient = sum_{u=k}^{n} binom(u,k) * (k+1)^n / (u+1) */
    seq_t num = 0, den = 1;
    for (idx_t u = k_col; u <= n_row; ++u) {
        seq_t term_num = binom(u, k_col) * power;
        seq_t term_den = (seq_t)(u + 1);

        seq_t g = seq_gcd(den, term_den);
        seq_t a1 = term_den / g;
        seq_t a2 = den / g;
        seq_t new_num = num * a1 + term_num * a2;
        seq_t new_den = den * a1;

        seq_t g2 = seq_gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;
    }

    return den;  /* denominator of the rational coefficient */
}
