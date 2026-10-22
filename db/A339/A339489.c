/*
 * A339489  T(n, k) = Product(divisors(k) union {k*j : j = 2..floor(n/k)}).
 *          Triangle read by rows.
 * https://oeis.org/A339489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula
   T(r, k) = ∏(divisors(k) ∪ {k·j : j = 2 .. ⌊r/k⌋}),
   with r = the smallest integer such that r(r+1)/2 ≥ n,
   and k = n - (r-1)r/2.
   a(n) = T(r, k).  (Triangle read by rows, offset 1)
*/

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* determine row r and column k for the n‑th term */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    idx_t k = n - (r * (r - 1)) / 2;        /* 1 ≤ k ≤ r */

    /* factorise k and collect prime exponents */
    idx_t tmp   = k;
    idx_t ps[64];
    idx_t es[64];
    size_t pcnt = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            ps[pcnt] = p;
            es[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {            /* remaining prime factor */
        ps[pcnt] = tmp;
        es[pcnt] = 1;
        ++pcnt;
    }

    /* total number of divisors d(k) = ∏ (e_i + 1) */
    idx_t d_k = 1;
    for (size_t i = 0; i < pcnt; ++i)
        d_k *= (es[i] + 1);

    /* product of all divisors of k
       = ∏ p_i^{ e_i * d(k) / 2 }   (integer exponent) */
    seq_t proddiv = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t e   = es[i];
        idx_t exp = e * d_k / 2;         /* integer exponent */
        proddiv *= big_pow((seq_t)ps[i], (seq_t)exp);
    }

    /* m = ⌊r/k⌋  (>= 1) */
    idx_t m = r / k;

    /* k^{m-1} */
    seq_t kpow = big_pow((seq_t)k, (seq_t)(m - 1));

    /* factorial of m  (m! ) */
    seq_t factm = factorial(m);

    return proddiv * kpow * factm;
}
