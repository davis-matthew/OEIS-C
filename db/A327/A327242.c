/*
 * A327242  Expansion of Sum_{k>=1} tau(k) * x^k / (1 + x^k)^2, where tau =
 *          A000005.
 * https://oeis.org/A327242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d+1) * d * tau(n/d)
 * Range:     i8 n <= 47, i16 n <= 3359, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void rec_divisors(int idx,
                         uint64_t cur,
                         seq_t tau_rem,
                         const uint64_t *primes,
                         const int *exps,
                         int cnt,
                         seq_t *sum)
{
    if (idx == cnt) {
        seq_t sign = (cur & 1ULL) ? 1 : -1;
        *sum += sign * (seq_t)cur * tau_rem;
        return;
    }
    int e = exps[idx];
    uint64_t p = primes[idx];
    seq_t div = (seq_t)(e + 1);
    uint64_t pow = 1;
    for (int k = 0; k <= e; ++k) {
        seq_t new_tau = tau_rem / div * (seq_t)(e - k + 1);
        rec_divisors(idx + 1, cur * pow, new_tau, primes, exps, cnt, sum);
        pow *= p;
    }
}

seq_t a(idx_t n)
{
    /* n is >=1 by definition of the sequence */
    uint64_t nn = (uint64_t)n;

    /* factorization */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;
    uint64_t temp = nn;

    /* factor 2 separately */
    if ((temp & 1ULL) == 0) {
        int e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++e;
        }
        primes[cnt] = 2;
        exps[cnt] = e;
        ++cnt;
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* initial tau_rem = product (e_i + 1) */
    seq_t tau_rem = 1;
    for (int i = 0; i < cnt; ++i) {
        tau_rem *= (seq_t)(exps[i] + 1);
    }

    seq_t sum = 0;
    rec_divisors(0, 1, tau_rem, primes, exps, cnt, &sum);

    return sum;
}
