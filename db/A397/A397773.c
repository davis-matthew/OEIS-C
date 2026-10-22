/*
 * A397773  a(n) is the number of distinct values of Sum_{x=0..n-1}
 *          exp(2*Pi*i*c*x^n/n^2) as c runs over the residues s^(2*(n-1)) mod
 *          n^2 with gcd(s,n) = 1.
 * https://oeis.org/A397773
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c * ∏_{p unitary} p * gcd(n/p, p−1) where n = 2^k0 *
 *            ∏_{p_s odd, exp=1} p_s * ∏_{q_t odd, exp≥2} q_t^k_t A = number
 *            of unitary primes p_s c = 1 if k0 = 0 2^(1−A) if k0 = 2 2^(−A)
 *            otherwise (k0 = 1 or k0 ≥ 3) In practice the factor c is applied
 *            by multiplying or dividing by powers of 2 afte...
 * Range:     u8 n <= 54, u16 n <= 1264, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Only defined for n ≥ 2, but we guard against smaller inputs. */
    if (n < 2)
        return 0;

    /* Factorization.  Use a fixed-size buffer; enough for 64‑bit numbers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Count exponent of 2 and unitary odd primes. */
    idx_t k0 = 0;          /* exponent of 2 */
    size_t A  = 0;          /* number of unitary odd primes */

    seq_t ans = (seq_t)1;  /* product over unitary primes */

    /* Original value of n for later divisions. */
    const idx_t n_orig = n;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2)
        {
            k0 = e;
        }
        else if (e == 1)          /* unitary prime */
        {
            ++A;
            seq_t pseq   = (seq_t)p;
            seq_t ndivp  = (seq_t)(n_orig / p);      /* n / p   */
            seq_t g      = gcd(ndivp, (seq_t)(p - 1)); /* gcd(n/p, p-1) */
            seq_t factor = pseq * g;                    /* p * g */
            ans *= factor;
        }
        /* Primes with exponent ≥2 are ignored in the product. */
    }

    /* Apply the correction factor c by multiplying or dividing by powers of 2. */
    if (k0 == 0)
    {
        /* No change. */
    }
    else if (k0 == 2)
    {
        if (A == 0)
            ans *= pow2((idx_t)1);          /* multiply by 2 */
        else if (A == 1)
            ;                               /* c = 1, nothing */
        else
            ans /= pow2((idx_t)(A - 1));    /* divide by 2^(A‑1) */
    }
    else /* k0 == 1 or k0 >= 3 */
    {
        if (A > 0)
            ans /= pow2((idx_t)A);          /* divide by 2^A */
    }

    return ans;
}
