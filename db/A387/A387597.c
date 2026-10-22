/*
 * A387597  Triangle read by rows: T(n,k) = Sum_{j=0..2k} (-1)^j *
 *          binomial(2k,j) * (k-j)^(2n).
 * https://oeis.org/A387597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(0) = 1; For n > 0 let m = ⌊(√(8·n + 1) – 1) / 2⌋ // largest t
 *            with t(t+1)/2 ≤ n t = m·(m + 1) / 2 // triangular number T_m If
 *            n = t then a(n) = 0 (triangular indices give zero). Otherwise
 *            let k = n – t (1 ≤ k ≤ m) and a(n) = ∑_{j=0}^{2k} (-1)^j ·
 *            binomial(2k, j) · (k – j)^(2·m). (This is T(m,k)...
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

static seq_t compute_T(idx_t m, idx_t k)
{
    seq_t pos = 0;
    seq_t neg = 0;
    idx_t two_k = 2 * k;
    idx_t exp  = 2 * m;             // exponent = 2m

    for (idx_t j = 0; j <= two_k; ++j)
    {
        seq_t bin  = binomial(two_k, j);      // (2k choose j)
        idx_t abs_base = (k >= j) ? (k - j) : (j - k);   // |k - j|
        seq_t powv  = pow_uint((uint64_t)abs_base, exp);  // (|k-j|)^(2m)
        seq_t term  = bin * powv;                 // term = bin * power

        if (j & 1)         // odd j → negative contribution
            neg += term;
        else
            pos += term;
    }

    return pos - neg;   // pos ≥ neg for all (m,k)
}

seq_t a(idx_t n)
{
    if (n == 0)                 // special case
        return 1;

    /* Find the largest m such that m*(m+1)/2 ≤ n.
       Since n ≤ 10000, a simple loop is sufficient and safe. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    idx_t Tm = m * (m + 1) / 2;
    if (Tm == n)                // n is a triangular number
        return 0;

    idx_t k = n - Tm;           // 1 ≤ k ≤ m
    return compute_T(m, k);
}
