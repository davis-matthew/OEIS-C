/*
 * A343252  a(n) is the least k0 <= n such that v_7(n), the 7-adic order of n,
 *          can be obtained by the formula: v_7(n) = log_7(n / L_7(k0, n)),
 *          where L_7(k0, n) is the lowest common denominator of the elements
 *          of the set S_7(k0, n) = {(1/n)*binomial(n, k), with 0 < k <= k0
 *          such that k is not divisible by 7} o...
 * https://oeis.org/A343252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest power of a prime different from 7 that divides
 *            n, or 1 if n has no prime factor other than 7
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1 the sequence is defined to be 1. */
    if (n <= 1) {
        return (seq_t)1;
    }

    idx_t temp = n;
    seq_t max_power = 1;      /* current maximum prime power found */

    /* Factor out the 2’s first so we can skip even numbers later. */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t pow2 = 1;
        for (idx_t i = 0; i < e; ++i) {
            pow2 *= 2;
        }
        if (pow2 > max_power) {
            max_power = pow2;
        }
    }

    /* Iterate over odd potential factors up to sqrt(temp).  Using
       temp / p instead of p * p keeps us out of overflow danger. */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (p != 7) {               /* ignore factor 7 */
                seq_t p_pow = 1;
                for (idx_t i = 0; i < e; ++i) {
                    p_pow *= (seq_t)p;
                }
                if (p_pow > max_power) {
                    max_power = p_pow;
                }
            }
        }
    }

    /* If anything is left, it is a prime factor > sqrt(original n). */
    if (temp > 1 && temp != 7) {
        seq_t p_pow = (seq_t)temp;
        if (p_pow > max_power) {
            max_power = p_pow;
        }
    }

    return max_power;
}
