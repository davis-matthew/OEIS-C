/*
 * A273289  Least prime not less than the median of all prime divisors of n
 *          counted with multiplicity.
 * https://oeis.org/A273289
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime not less than the median of all prime
 *            divisors of n counted with multiplicity
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper prototypes (provided by the build system). */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n,
                                  uint64_t *pr, uint64_t *ex,
                                  size_t *cnt); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */

/* Compute a(n).  For n < 2 the sequence is undefined; return 0. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    uint64_t nn = (uint64_t)n;

    /* Prime factorisation of n.  The helper returns distinct primes
       in ascending order.  128 slots are far more than needed for all
       reasonable n in the test suite. */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* Total number of prime factors (with multiplicity). */
    size_t total = 0;
    for (size_t i = 0; i < cnt; ++i)
        total += exps[i];

    /* Find the median (or the two central primes when total is even). */
    uint64_t median = 0;

    if (total & 1) {                 /* odd number of factors */
        size_t target = total / 2;    /* zero‑based index of the median */
        size_t cum = 0;
        for (size_t i = 0; i < cnt; ++i) {
            cum += exps[i];
            if (cum > target) {
                median = primes[i];
                break;
            }
        }
    } else { /* even number of factors – need the mean of the two central values */
        size_t target1 = total / 2 - 1;
        size_t target2 = total / 2;
        size_t cum = 0;
        uint64_t a = 0, b = 0;
        for (size_t i = 0; i < cnt; ++i) {
            cum += exps[i];
            if (cum > target1 && a == 0) a = primes[i];
            if (cum > target2) { b = primes[i]; break; }
        }
        uint64_t sum = a + b;
        median = (sum + 1) / 2;       /* ceil((a+b)/2) */
    }

    /* Find the least prime not less than median. */
    seq_t ret;
    if (median <= 2)
        ret = 2;
    else if (is_prime_big(median))
        ret = (seq_t)median;
    else
        ret = (seq_t)next_prime(median);

    return ret;
}
