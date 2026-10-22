/*
 * A374777  Numerator of the mean abundancy index of the divisors of n.
 * https://oeis.org/A374777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the mean abundancy index of the divisors of
 *            n a(n) = (Σ_{d|n} d·τ(d)) / gcd(Σ_{d|n} d·τ(d) , n·τ(n)) ( τ is
 *            the divisor count )
 * Range:     u8 n <= 22, u16 n <= 1619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Edge case – the OEIS offset starts at 1, but we guard against n==0. */
    if (n == 0) return 0;

    seq_t S_product = 1;            /* Σ_{d|n} d·τ(d)  */
    seq_t D_product = 1;            /* n·τ(n)           */

    uint64_t tmp = (uint64_t)n;
    /* Trial division – sufficient for the required range (up to 10^4 or a bit more). */
    for (uint64_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }

            /* Compute S(p^e) = Σ_{k=0}^{e} p^k·(k+1) */
            seq_t power = 1;          /* p^0 */
            seq_t sum   = 0;
            for (uint64_t k = 0; k <= e; ++k)
            {
                sum += power * (seq_t)(k + 1);
                power *= (seq_t)p;    /* power becomes p^{k+1} for next iteration */
            }
            /* After the loop, power = p^{e+1}.  Compute p^e = power / p. */
            seq_t p_pow_e = power / (seq_t)p;
            seq_t D_pe    = p_pow_e * (seq_t)(e + 1);   /* p^e·(e+1) */

            S_product *= sum;
            D_product *= D_pe;
        }
    }

    /* Remaining prime factor, if any, has exponent 1. */
    if (tmp > 1)
    {
        uint64_t p = tmp;
        uint64_t e = 1;   /* exponent is 1 */

        seq_t power = 1;
        seq_t sum   = 0;
        for (uint64_t k = 0; k <= e; ++k)
        {
            sum += power * (seq_t)(k + 1);
            power *= (seq_t)p;
        }
        seq_t p_pow_e = power / (seq_t)p;  /* p^1 = tmp itself */
        seq_t D_pe    = p_pow_e * (seq_t)(e + 1);

        S_product *= sum;
        D_product *= D_pe;
    }

    /* Reduce the fraction S_product / D_product to lowest terms. */
    seq_t a_val = S_product;
    seq_t b_val = D_product;
    while (b_val != 0)
    {
        seq_t t = a_val % b_val;
        a_val = b_val;
        b_val = t;
    }
    seq_t g = a_val;   /* gcd */

    return S_product / g;
}
