/*
 * A347104  Dirichlet g.f.: primezeta(s-1) * zeta(s-1) / zeta(s).
 * https://oeis.org/A347104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} p * φ(n/p) where φ is Euler's totient function.
 *            The implementation factors n, computes φ(n) once, then uses
 *            φ(n/p) = φ(n)/p if the exponent of p in n ≥ 2 or φ(n/(p)) =
 *            φ(n)/(p-1) if the exponent is 1
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _pow_seqt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/_pow_seqt.h"

/* Helper: factor n into prime powers.
   Primes[] and exps[] must be large enough.
   Returns the count of distinct primes. */
static size_t _factor_uint64(uint64_t n, uint64_t primes[], uint64_t exps[])
{
    size_t cnt = 0;
    uint64_t temp = n;

    /* Handle factor 2 separately */
    if ((temp & 1ULL) == 0) {
        uint64_t e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        primes[cnt] = 2ULL;
        exps[cnt] = e;
        ++cnt;
    }

    /* Handle odd factors */
    for (uint64_t d = 3ULL; d * d <= temp; d += 2ULL) {
        if (temp % d == 0) {
            uint64_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            primes[cnt] = d;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1ULL) {             /* remaining prime > sqrt(original n) */
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Euler's totient for uint64_t, returned as seq_t. */
static seq_t _phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    seq_t phi = 1;
    uint64_t temp = n;
    uint64_t primes[64], exps[64];
    size_t cnt = _factor_uint64(temp, primes, exps);
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        /* φ(p^e) = p^(e-1) * (p-1) */
        seq_t term = _pow_seqt(p, e - 1);
        term *= (seq_t)(p - 1);
        phi *= term;
    }
    return phi;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    uint64_t temp = (uint64_t)n;
    size_t cnt = _factor_uint64(temp, primes, exps);

    /* Compute φ(n) once */
    seq_t phi_n = _phi_uint64(temp);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        seq_t phi_np;
        if (e == 1)
            phi_np = phi_n / (p - 1);
        else
            phi_np = phi_n / p;
        sum += p * phi_np;
    }

    return sum;
}
