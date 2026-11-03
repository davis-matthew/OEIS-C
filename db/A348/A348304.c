/*
 * A348304  a(n) = Sum_{d|n} d'', where d'' is the second arithmetic
 *          derivative of d (A068346).
 * https://oeis.org/A348304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} d'' where d'' is the second arithmetic
 *            derivative of d The code factors each divisor d, computes its
 *            first derivative d' by d' = Σ (n/p_i) * e_i for the prime
 *            factorization d = ∏ p_i^{e_i}, then computes the second
 *            derivative d'' = (d')' by the same routine. Finally it sums d''
 *            o...
 * Range:     u8 n <= 31, u16 n <= 1535, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 9883 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/*--- Compute the first arithmetic derivative of an unsigned integer n ---*/
static seq_t first_derivative_u64(uint64_t n)
{
    if (n <= 1)            /* 0' = 0, 1' = 0 */
        return 0;
    seq_t res = 0;
    uint64_t temp = n;

    /* Trial division to factor n */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            /* Contribution of this prime factor: (n/p) * e  */
            uint64_t n_over_p = n / p;
            res += (seq_t)n_over_p * (seq_t)e;
        }
    }
    if (temp > 1)          /* Remaining prime factor with e = 1 */
    {
        uint64_t n_over_p = n / temp;
        res += (seq_t)n_over_p;   /* e = 1 */
    }
    return res;
}

/*--- Compute the second arithmetic derivative of an unsigned integer n ---*/
static seq_t second_derivative_u64(uint64_t n)
{
    seq_t first  = first_derivative_u64(n);
    /* first is guaranteed to fit into 64 bits for the problem size (n ≤ 10 000) */
    seq_t second = first_derivative_u64((uint64_t)first);
    return second;
}

/*--- Main sequence function -----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)   /* OEIS offset is 1; behaviour for n=0 is irrelevant */
        return 0;

    seq_t total = 0;
    /* Enumerate all divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            total += second_derivative_u64(d);
            idx_t e = n / d;
            if (e != d)          /* avoid double‑counting perfect square root */
                total += second_derivative_u64(e);
        }
    }
    return total;
}
