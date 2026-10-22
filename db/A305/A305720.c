/*
 * A305720  Square array T(n, k) read by antidiagonals, n > 0 and k > 0; for
 *          any prime number p, the p-adic valuation of T(n, k) is the product
 *          of the p-adic valuations of n and of k.
 * https://oeis.org/A305720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, mult
 * Formula:   * a(n) = T(i,j) where the antidiagonal coordinates (i,j) are * i
 *            = n - (s-1)*s/2 * j = s - i + 1 * with s = smallest integer
 *            satisfying s*(s+1)/2 >= n * T(i,j) is the product over all
 *            primes p of p^{v_p(i)*v_p(j)} * (v_p(x) is the exponent of p in
 *            the prime factorisation of x)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* Factor n into primes and exponents.  Results are stored in the
 * supplied arrays of size 64.  *cnt is set to the number of factors. */
static void factor_small(idx_t n, idx_t primes[64], unsigned exp[64], size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[*cnt] = p;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[*cnt] = n;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* Compute T(i,j) = Π p^{v_p(i)*v_p(j)}.
 * It uses the multiplicativity of the function; for i=1 or j=1 the
 * result is 1. */
static seq_t compute_T(idx_t i, idx_t j)
{
    if (i == 1 || j == 1) return 1;

    idx_t primes_i[64], primes_j[64];
    unsigned exp_i[64], exp_j[64];
    size_t cnt_i, cnt_j;

    factor_small(i, primes_i, exp_i, &cnt_i);
    factor_small(j, primes_j, exp_j, &cnt_j);

    seq_t result = 1;

    for (size_t a = 0; a < cnt_i; ++a) {
        idx_t p = primes_i[a];
        unsigned e_i = exp_i[a];
        unsigned e_j = 0;

        for (size_t b = 0; b < cnt_j; ++b) {
            if (primes_j[b] == p) {
                e_j = exp_j[b];
                break;
            }
        }
        if (e_j == 0) continue;

        unsigned e = e_i * e_j;
        seq_t mul = 1;
        for (unsigned k = 0; k < e; ++k) mul *= (seq_t)p;
        result *= mul;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal number s such that
     *  s*(s+1)/2 >= n  and  (s-1)*s/2 < n .
     */
    idx_t s = (idx_t)((sqrt((long double)(8 * n + 1)) - 1) / 2);

    /* Correct for possible floating‑point rounding error. */
    while (s * (s + 1) / 2 < n) ++s;
    while ((s - 1) * s / 2 >= n) --s;

    idx_t offset = n - (s - 1) * s / 2;
    idx_t i = offset;
    idx_t j = s - offset + 1;

    return compute_T(i, j);
}
