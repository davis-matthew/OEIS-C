/*
 * A324981  a(n) = Product_{d|n} (d*pod(d)) where pod(k) = the product of the
 *          divisors of k (A007955).
 * https://oeis.org/A324981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} (d * pod(d))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 23, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where pod(k) is the product of the divisors of k. */

/* The implementation follows a recursive enumeration of all divisors of n,
 * computing for each divisor d the factor d·pod(d) and multiplying them
 * together.  We keep track of the divisor d, its number of divisors
 * tau(d), and the square–root part of d (needed when tau(d) is odd).
 */

#include <stdint.h>
#include <stddef.h>

/* Fast exponentiation: res = base^exp (exp is an idx_t). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp != 0)
    {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Recursive enumeration of all divisors of n.
 * pr,exps: arrays of primes and their exponents.
 * m          : number of distinct primes.
 * pos        : current prime index.
 * cur_d      : current divisor.
 * cur_tau    : product of (f_i+1) for the current divisor.
 * cur_sqrt   : product of p_i^{floor(f_i/2)} for the current divisor.
 * res        : running product of a(n).
 */
static void rec(const uint64_t *pr, const uint64_t *exps, size_t m,
                size_t pos, seq_t cur_d, idx_t cur_tau,
                seq_t cur_sqrt, seq_t *res)
{
    if (pos == m)
    {
        idx_t e_term = cur_tau / 2 + 1;   /* floor(tau/2)+1  */
        seq_t term   = pow_seq(cur_d, e_term);
        if (cur_tau & 1)                 /* tau odd → multiply by sqrt(d) */
            term *= cur_sqrt;
        *res *= term;
        return;
    }

    uint64_t p    = pr[pos];
    uint64_t maxe = exps[pos];
    seq_t   pow_p = 1;   /* p^e for the current e */
    seq_t   pow_sqrt = 1;/* p^{floor(e/2)}             */

    for (uint64_t e = 0; e <= maxe; ++e)
    {
        seq_t   new_d       = cur_d * pow_p;
        seq_t   new_sqrt    = cur_sqrt * pow_sqrt;
        idx_t   new_tau     = cur_tau * (e + 1);

        rec(pr, exps, m, pos + 1, new_d, new_tau, new_sqrt, res);

        pow_p *= (seq_t)p;                     /* next p^e          */
        if (((e + 1) & 1) == 0)                /* floor((e+1)/2)++ */
            pow_sqrt *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* safety guard – Offset is 1 */
        return 0;

    uint64_t tn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pcnt = 0;

    /* Simple trial division factorisation.  n ≤ 10000, fast enough. */
    for (uint64_t p = 2; p * p <= tn; ++p)
    {
        if (tn % p == 0)
        {
            uint64_t e = 0;
            while (tn % p == 0)
            {
                tn /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tn > 1)     /* remaining prime factor */
    {
        primes[pcnt] = tn;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    seq_t result = 1;
    rec(primes, exps, pcnt, 0, (seq_t)1, (idx_t)1, (seq_t)1, &result);
    return result;
}
