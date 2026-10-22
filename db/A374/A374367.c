/*
 * A374367  a(n) = Im(A008683(n)*(i^n)).
 * https://oeis.org/A374367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = mu(n)*Im(i^n), where Im(i^n) = 1 if n≡1 (mod4), * Im(i^n)
 *            = -1 if n≡3 (mod4), * Im(i^n) = 0 if n even. The mu(n) factor *
 *            is the Möbius function: mu(n)=0 * if n has a squared prime
 *            factor, else * mu(n)=(-1)^{ω(n)} (ω = number of distinct primes)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Ensure n≥1 per OEIS offset; return 0 for n==0 as safety. */
    if (n == 0)
        return 0;

    /* Even indices always give 0 because Im(i^n)=0. */
    if ((n & 1ULL) == 0ULL)
        return 0;

    idx_t temp = n;
    int mu_sign = 1;          /* current sign of μ(n) */
    int prime_cnt = 0;        /* count of distinct prime factors */

    /* Trial division up to sqrt(temp). Use division to avoid overflow. */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                if (exp > 1)       /* squared prime factor → μ = 0 */
                    return 0;
            }
            ++prime_cnt;
            mu_sign = -mu_sign;   /* flip sign for each distinct prime */
        }
    }

    /* Remaining prime factor (if any) > sqrt(original n). */
    if (temp > 1) {
        ++prime_cnt;
        mu_sign = -mu_sign;
    }

    /* μ(n) is 0 only if a squared factor was found (handled above). */
    seq_t mu = (seq_t)mu_sign;          /* μ(n) = ±1 */

    /* Im(i^n) for odd n: 1 if n≡1 (mod4), -1 if n≡3 (mod4). */
    int im_part = (n % 4 == 1) ? 1 : -1;

    return mu * (seq_t)im_part;
}
