/*
 * A372997  a(n) = Sum_{k=1..n} tau( (n/gcd(k,n))^3 ).
 * https://oeis.org/A372997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9505 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d|n} phi(d) * tau(d^3)
 *
 * This implementation factorises n, enumerates all divisors using a
 * recursive DFS over the prime factorisation and accumulates
 * phi(d) and tau(d^3) multiplicatively.  No floating point or large
 * intermediate types are required; all arithmetic uses `seq_t`.
 */

#include <stddef.h>
#include <stdint.h>

/*---------------------------------------------------------------------------*
 *  Helper: recursive divisor enumeration
 *---------------------------------------------------------------------------*/
static void div_rec(const uint64_t *prm, const uint64_t *fld,
                    size_t cnt, size_t idx,
                    seq_t phi_prod, seq_t tau_prod,
                    seq_t *sum)
{
    if (idx == cnt) {
        *sum += phi_prod * tau_prod;
        return;
    }

    uint64_t p   = prm[idx];
    uint64_t exp = fld[idx];

    seq_t p_pow      = 1;  /* p^f               */
    seq_t p_pow_prev = 1;  /* p^(f-1)           */

    for (uint64_t f = 0; f <= exp; ++f) {
        seq_t new_phi = phi_prod;
        seq_t new_tau = tau_prod * (seq_t)(3 * f + 1);

        if (f > 0) {
            /* phi(p^f) = p^f - p^(f-1) */
            seq_t phi_factor = p_pow - p_pow_prev;
            new_phi *= phi_factor;
        }

        div_rec(prm, fld, cnt, idx + 1, new_phi, new_tau, sum);

        /* prepare for next f */
        p_pow_prev = p_pow;
        p_pow *= (seq_t)p;
    }
}

/*---------------------------------------------------------------------------*
 *  Helper: factorise unsigned 64‑bit integer
 *---------------------------------------------------------------------------*/
static void factor_uint64(uint64_t n, uint64_t *prm,
                          uint64_t *fld, size_t *cnt)
{
    size_t c = 0;
    if (n % 2 == 0) {
        uint64_t e = 0;
        while (n % 2 == 0) {
            n /= 2;
            ++e;
        }
        prm[c]   = 2;
        fld[c++] = e;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            prm[c]   = p;
            fld[c++] = e;
        }
    }
    if (n > 1) {
        prm[c]   = n;
        fld[c++] = 1;
    }
    *cnt = c;
}

/*---------------------------------------------------------------------------*
 *  Main sequence function
 *---------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    uint64_t nn = (uint64_t)n;

    /* factorise n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t prime_cnt;
    factor_uint64(nn, primes, exps, &prime_cnt);

    /* enumerate divisors and accumulate */
    seq_t sum = (seq_t)0;
    div_rec(primes, exps, prime_cnt, 0, (seq_t)1, (seq_t)1, &sum);

    return sum;
}
