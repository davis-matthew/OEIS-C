/*
 * A378981  a(n) = (A003961(n)-sigma(n)) mod (A003961(n)-2*n), where A003961
 *          is fully multiplicative with a(prime(i)) = prime(i+1).
 * https://oeis.org/A378981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A003961(n)-σ(n)) mod (A003961(n)-2*n)
 * Range:     u8 n <= 16, u16 n <= 1128, u32 n <= 9140, u64 n <= 9140,
 *            u128 n <= 9140, big any n
 * Helpers:   factor_u64, next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A003961 is completely multiplicative with
 *  a(prime(i)) = prime(i+1).  For a prime power p^e this gives
 *  A003961(p^e) = next_prime(p)^e.
 *
 *  σ(n) is the sum of divisors of n.  For n = ∏p_i^e_i,
 *  σ(n) = ∏(p_i^(e_i+1)-1)/(p_i-1).
 *
 *  Both functions are computed from the prime factorisation of n.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"

/* Compute A003961(n) from a prime factorisation. */
static seq_t
A003961_from_factorisation(const uint64_t *fprimes,
                           const uint64_t *fexps,
                           size_t fcnt)
{
    seq_t val = 1;
    for (size_t i = 0; i < fcnt; ++i)
    {
        uint64_t p   = fprimes[i];
        uint64_t e   = fexps[i];
        uint64_t np  = next_prime(p);
        seq_t part   = pow_uint(np, e);
        val *= part;
    }
    return val;
}

/* Compute σ(n) from a prime factorisation. */
static seq_t
sigma_from_factorisation(const uint64_t *fprimes,
                         const uint64_t *fexps,
                         size_t fcnt)
{
    seq_t val = 1;
    for (size_t i = 0; i < fcnt; ++i)
    {
        uint64_t p = fprimes[i];
        uint64_t e = fexps[i];
        seq_t numer = pow_uint(p, e + 1) - 1;
        seq_t denom = (seq_t)(p - 1);
        seq_t term  = numer / denom;          /* exact division */
        val *= term;
    }
    return val;
}

seq_t a(idx_t n)
{
    /* Factorise n.  The helper writes at most 64 factors. */
    uint64_t fprimes[64];
    uint64_t fexps[64];
    size_t   fcnt = 0;
    factor_u64((uint64_t)n, fprimes, fexps, &fcnt);

    /* A003961(n) */
    seq_t ap = A003961_from_factorisation(fprimes, fexps, fcnt);

    /* σ(n) */
    seq_t sigma = sigma_from_factorisation(fprimes, fexps, fcnt);

    /* Compute the divisor of the modulus without overflow. */
    seq_t two_n = (seq_t)n * 2U;           /* safely in unsigned seq_t */

    /* abs_diff2 = |A003961(n) - 2*n| */
    seq_t abs_diff2 = (ap >= two_n) ? (ap - two_n) : (two_n - ap);

    if (abs_diff2 == 0)
        return 0;                          /* as defined in OEIS */

    /* Compute (A003961(n)-σ(n)) modulo the absolute value of the divisor. */
    seq_t diff1 = ap - sigma;              /* unsigned, may wrap – OK for modulo */
    return diff1 % abs_diff2;
}
