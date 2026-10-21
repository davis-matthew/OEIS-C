/*
 * A268336  a(n) = A174824(n)/n, where A174824(n) = lcm(A002322(n), n) and
 *          A002322(n) is the Carmichael lambda function (also known as the
 *          reduced totient function or the universal exponent of n).
 * https://oeis.org/A268336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(lambda(n), n)/n where lambda(n) is the Carmichael
 *            function A002322
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   factor_u64, gcd, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/gcd.h"

static seq_t _lambda(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;        /* λ(1) = 1 */

    /* Factorization: at most 64 distinct primes for uint64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t lam = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t    e = (idx_t)exps[i];
        seq_t term;

        if (p == 2ULL) {
            if (e == 1)
                term = (seq_t)1;
            else if (e == 2)
                term = (seq_t)2;
            else
                term = seq_pow((seq_t)2, e - 2);   /* 2^(e-2) for e≥3 */
        } else {
            seq_t pow_e = seq_pow((seq_t)p, e - 1);   /* p^(e-1) */
            term      = seq_mul(pow_e, (seq_t)(p - 1)); /* (p-1)*p^(e-1) */
        }

        /* lam = lcm(lam, term) = lam/gcd(lam,term) * term */
        seq_t g   = gcd(lam, term);
        lam     = seq_mul(lam / g, term);
    }

    return lam;
}

seq_t a(idx_t n)
{
    seq_t lam  = _lambda(n);
    seq_t nseq = (seq_t)n;
    seq_t g    = gcd(lam, nseq);        /* gcd(λ(n), n) */
    return lam / g;                     /* λ(n)/gcd(λ(n),n) = lcm(λ(n),n)/n */
}
