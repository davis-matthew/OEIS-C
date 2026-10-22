/*
 * A321466  Expansion of (phi(x^3)^3 / phi(x))^2 in powers of x where phi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A321466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 For n > 0: a(n) = -4 * ( σ(n) - 6 σ(n/2) + σ(n/3) + 4
 *            σ(n/4) + 2 σ(n/6) + 4 σ(n/12) ) where σ(k) = sum of divisors of
 *            k (σ(k)=0 if k is not an integer)
 * Range:     i8 n <= 19, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

/* compute the sum‑of‑divisors function σ(m) for m ≥ 1 */
static seq_t sigma_uint64(idx_t m)
{
    if (m == 0) return 0;
    seq_t result = 1;
    uint64_t temp   = (uint64_t)m;
    uint64_t p;
    for (p = 2; p * p <= temp; p += (p == 2 ? 1 : 2))
    {
        if (temp % p == 0)
        {
            seq_t term = 1;               /* geometric series for prime p */
            seq_t pp   = 1;
            while (temp % p == 0)
            {
                temp /= p;
                pp *= (seq_t)p;
                term += pp;
            }
            result *= term;
        }
    }
    if (temp > 1)          /* remaining prime factor */
        result *= (1 + (seq_t)temp);
    return result;
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sn   = sigma_uint64(n);
    seq_t sn2  = 0;
    seq_t sn3  = 0;
    seq_t sn4  = 0;
    seq_t sn6  = 0;
    seq_t sn12 = 0;

    if (n % 2 == 0)  sn2  = sigma_uint64(n / 2);
    if (n % 3 == 0)  sn3  = sigma_uint64(n / 3);
    if (n % 4 == 0)  sn4  = sigma_uint64(n / 4);
    if (n % 6 == 0)  sn6  = sigma_uint64(n / 6);
    if (n % 12 == 0) sn12 = sigma_uint64(n / 12);

    seq_t val = sn;
    val -= (seq_t)6 * sn2;
    val += sn3;
    val += (seq_t)4 * sn4;
    val += (seq_t)2 * sn6;
    val += (seq_t)4 * sn12;
    val *= (seq_t)(-4);

    return val;
}
