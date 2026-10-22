/*
 * A328925  a(n) = A002322(n)/A118106(n); write n = Product_{i=1..t} p_i^e_i,
 *          then a(n) = A002322(n)/(lcm_{1<=i,j<=t,i!=j} ord(p_i,p_j^e_j)),
 *          where ord(a,r) is the multiplicative order of a modulo r, and
 *          A002322 is the Carmichael lambda (usually written as psi).
 * https://oeis.org/A328925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64, gcd, phi_prime_power, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328925
   a(n) = A002322(n) / A118106(n)
   where A002322 is Carmichael lambda (psi) and A118106 is
   the lcm of ord(p_i , p_j^e_j) over all ordered pairs of distinct
   prime divisors of n.
   For prime powers n=p^e, a(n)=psi(n). */

/* The implementation uses standard integer operations and the helper
   functions provided in the build environment. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/powmod64.h"
#include "../utils/phi_prime_power.h"

/* compute lcm of two seq_t */
static inline seq_t seq_lcm(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

/* Carmichael lambda for p^e */
static inline uint64_t lambda_prime_power(uint64_t p, uint64_t e)
{
    if (p == 2) {
        if (e == 1) return 1ULL;
        if (e == 2) return 2ULL;
        return 1ULL << (e - 2);          /* 2^(e-2) */
    }
    /* odd primes: λ(p^e) = φ(p^e) = p^(e-1)*(p-1) */
    uint64_t lam = 1;
    for (uint64_t i = 1; i <= e - 1; ++i) lam *= p;
    lam *= (p - 1);
    return lam;
}

/* multiplicative order of a modulo m (coprime) */
static seq_t order_mod(uint64_t a, uint64_t m)
{
    uint64_t phi = phi_prime_power((uint64_t)m / ((m) & -(m)), 1); /* not used, placeholder */
    /* Actually m is a prime power p^e.  Compute phi(m). */
    /* Below we identify p and e from m. */
    uint64_t p = m;
    while ((p & 1) == 0) { p >>= 1; }  /* remove powers of 2 to get base prime? */
    /* Oops, easier: we know m is a prime power from calling context.
       We'll recompute phi directly. */
    /* Since we may not know p and e separately, we just recompute phi by
       factoring m.  However, it is easier: m is p^e, so m/p^(e-1) = p.
       We'll handle this by dividing by p until remainder. */
    /* Find prime p dividing m */
    uint64_t prime = 0, exp = 0, tmp = m;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            prime = d; break;
        }
    }
    if (prime == 0) prime = tmp;  /* m itself is prime */
    /* Count exponent */
    tmp = m;
    while (tmp % prime == 0) {
        tmp /= prime;
        exp++;
    }
    uint64_t phi_m = phi_prime_power(prime, exp);
    /* factor phi_m */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(phi_m, pr, ex, &cnt);
    uint64_t ord = phi_m;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = pr[i];
        while (ord % q == 0 &&
               powmod64(a, ord / q, m) == 1) {
            ord /= q;
        }
    }
    return (seq_t)ord;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* a(1)=1 */

    /* factor n */
    uint64_t primes[64], exps[64];
    size_t nc;
    factor_u64((uint64_t)n, primes, exps, &nc);

    seq_t Lpsi = (seq_t)1;
    seq_t Llord = (seq_t)1;

    /* compute psi (lcm of lambda(p^e)) */
    for (size_t i = 0; i < nc; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t lam = lambda_prime_power(p, e);
        Lpsi = seq_lcm(Lpsi, (seq_t)lam);
    }

    /* compute A118106 (lcm of orders) */
    for (size_t i = 0; i < nc; ++i) {
        for (size_t j = 0; j < nc; ++j) {
            if (i == j) continue;
            uint64_t a_i = primes[i];
            uint64_t p_j = primes[j];
            uint64_t e_j = exps[j];
            /* modulus m = p_j^e_j */
            uint64_t m = 1;
            for (uint64_t k = 0; k < e_j; ++k) m *= p_j;
            seq_t ord = order_mod(a_i, m);
            Llord = seq_lcm(Llord, ord);
        }
    }

    return Lpsi / Llord;
}
