/*
 * A348349  a(n) = Sum_{d|n} d^(tau(d) - 1).
 * https://oeis.org/A348349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(tau(d) - 1) where tau(d) is the number of
 *            divisors of d
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursive helper to enumerate all divisors of n and accumulate the sum. */
static void rec_sum(size_t idx,
                    seq_t curd, seq_t curtau,
                    const uint64_t *primes, const uint64_t *exps, size_t cnt,
                    seq_t *sum)
{
    if (idx == cnt) {
        /* For divisor curd, tau(curd) = curtau.
           Add curd^(curtau - 1) to the sum. */
        seq_t term = big_pow(curd, curtau - 1);
        *sum += term;
        return;
    }

    /* Current prime and its exponent in n. */
    uint64_t p = primes[idx];
    uint64_t e = exps[idx];

    seq_t pow = 1;      /* p^f */
    for (uint64_t f = 0; f <= e; ++f) {
        seq_t new_tau = curtau * (f + 1);
        rec_sum(idx + 1,
                curd * pow, new_tau,
                primes, exps, cnt, sum);
        pow *= p; /* prepare p^(f+1) for next iteration */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* Undefined for 0, return 0 */

    /* Factorize n. 10000 has fewer than 20 distinct prime factors. */
    uint64_t primes_arr[20];
    uint64_t exps_arr[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes_arr, exps_arr, &cnt);

    seq_t sum = 0;
    rec_sum(0, (seq_t)1, (seq_t)1, primes_arr, exps_arr, cnt, &sum);
    return sum;
}
