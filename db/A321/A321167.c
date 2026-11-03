/*
 * A321167  The e-unitary Euler function: a(1) = 1, a(n) = Product uphi(e(i))
 *          for n = Product p(i)^e(i), where uphi is the unitary totient
 *          function (A047994).
 * https://oeis.org/A321167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e ∥ n} Π_{q^β ∥ e} (q^β – 1) (with a(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9627 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It factorizes n, then for each exponent e computes the unitary totient
   uphi(e) as the product of (q^β – 1) over the prime powers q^β dividing e. */

#include <stdint.h>
#include <stdbool.h>

/* Compute uphi(e) = ∏_{q^β ∥ e} (q^β – 1) */
static seq_t uphi_of_exponent(idx_t e)
{
    seq_t res = 1;
    idx_t tmp = e;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t pow = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pow *= p;
            }
            res *= (pow - 1);
        }
    }
    if (tmp > 1) {
        /* tmp is prime */
        res *= (tmp - 1);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1u) return 1;
    seq_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t uphi_e = uphi_of_exponent(e);
            result *= uphi_e;
        }
    }
    /* If m > 1, it is a prime factor with exponent 1 → uphi(1)=1,
       so result is unchanged. */
    return result;
}
