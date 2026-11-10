/*
 * A326140  a(n) = gcd(A318878(n), A318879(n)).
 * https://oeis.org/A326140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A318878(n), A318879(n)), where A318878(n) = Sum_{d|n,
 *            2*phi(d)-d>0} (2*phi(d)-d) and A318879(n) = Sum_{d|n,
 *            d-2*phi(d)>0} (d-2*phi(d))
 * Range:     u8 n <= 256, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 110 terms through n = 9882 (2026-11-08)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/totient.h"
#include "../utils/gcd.h"

static seq_t A318878_positive_f(idx_t d)
{
    seq_t phi_val = totient(d);
    seq_t diff = (seq_t)d - phi_val;
    if (phi_val > (seq_t)(d / 2))
    {
        return phi_val - diff;
    }
    return 0;
}

static seq_t A318878_a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            sum += A318878_positive_f(i);
            idx_t j = n / i;
            if (j != i)
                sum += A318878_positive_f(j);
        }
    }
    return sum;
}

static void
A318879_enum_divs(int idx,
                  seq_t cur_d,
                  seq_t cur_phi,
                  const uint64_t *primes,
                  const int *exps,
                  int np,
                  seq_t *sum)
{
    if (idx == np) {
        if (cur_d > (cur_phi << 1))
            *sum += cur_d - (cur_phi << 1);
        return;
    }

    A318879_enum_divs(idx + 1, cur_d, cur_phi, primes, exps, np, sum);

    uint64_t p     = primes[idx];
    seq_t    power = 1;
    seq_t    phi_pk = 1;
    for (int k = 1; k <= exps[idx]; ++k) {
        power *= (seq_t)p;
        if (k == 1)
            phi_pk = (seq_t)(p - 1);
        else
            phi_pk *= (seq_t)p;

        A318879_enum_divs(idx + 1,
                          cur_d * power,
                          cur_phi * phi_pk,
                          primes,
                          exps,
                          np,
                          sum);
    }
}

static seq_t A318879_a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t N = (uint64_t)n;

    uint64_t primes[64];
    int exps[64];
    int nprimes = 0;
    uint64_t tmp = N;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[nprimes]  = p;
            exps[nprimes]    = e;
            ++nprimes;
        }
    }
    if (tmp > 1) {
        primes[nprimes]  = tmp;
        exps[nprimes]    = 1;
        ++nprimes;
    }

    seq_t sum = 0;
    A318879_enum_divs(0, (seq_t)1, (seq_t)1, primes, exps, nprimes, &sum);
    return sum;
}

seq_t a(idx_t n)
{
    seq_t x = A318878_a(n);
    seq_t y = A318879_a(n);
    return gcd(x, y);
}
