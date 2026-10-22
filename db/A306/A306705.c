/*
 * A306705  a(n) = Product_{d|n} d*tau(d), where tau(k) = the number of the
 *          divisors of k (A000005).
 * https://oeis.org/A306705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} d*tau(d) (tau = number of divisors)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 95, u128 n <= 335,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: recursively enumerate all divisors of n given its prime factorisation */
/* primes[]   : array of distinct prime factors of n          */
/* exps[]     : corresponding exponents of these primes       */
/* cnt        : number of distinct prime factors              */
/* idx        : current depth in recursion                     */
/* current_d  : product of chosen prime powers so far        */
/* current_tau: product of (exponent+1) for chosen primes    */
/* result     : accumulated output value                      */
static void
enumerate_divisors(const uint64_t primes[], const int exps[],
                   int idx, int cnt,
                   seq_t current_d, seq_t current_tau,
                   seq_t *result)
{
    if (idx == cnt) {
        seq_t prod = current_d * current_tau;
        *result *= prod;
        return;
    }

    uint64_t p = primes[idx];
    seq_t pow = 1;           /* p^k  for current exponent k */
    for (int k = 0; k <= exps[idx]; ++k) {
        seq_t d     = current_d * pow;
        seq_t tau   = current_tau * (seq_t)(k + 1);
        enumerate_divisors(primes, exps, idx + 1, cnt, d, tau, result);
        pow *= p;
    }
}

/* Main function to compute a(n) */
seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the specification of the sequence */
    uint64_t val = (uint64_t)n;

    /* Factorise n into primes and exponents.
       For n <= 10000 a fixed-size array of 64 elements is more than enough. */
    uint64_t primes[64];
    int exps[64];
    int cnt = 0;

    uint64_t temp = val;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1) {      /* remaining prime factor */
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t result = (seq_t)1;
    enumerate_divisors(primes, exps, 0, cnt, (seq_t)1, (seq_t)1, &result);
    return result;
}
