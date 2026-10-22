/*
 * A384248  The sum of the integers from 1 to n whose largest divisor that is
 *          an infinitary divisor of n is 1.
 * https://oeis.org/A384248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = total_sum(n) - sum_{d|n, d is forbidden} sum of multiples
 *            of d A forbidden divisor is any prime power p^(t) where p
 *            divides n and t = 2^{v2(exp_p(n))} (the smallest power of 2
 *            dividing the exponent of p in n). Each number i (1 ≤ i ≤ n) is
 *            excluded iff it is divisible by at least one forbid...
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* External types provided by the build system */
typedef uint64_t idx_t;

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }

    /* factorize n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* build list of forbidden divisors (prime powers ≤ n) */
    uint64_t forbid[20];
    size_t fcnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t a_exp = exps[i];
        /* find smallest power of 2 dividing a_exp */
        uint64_t t = 1;
        while ((a_exp & 1ULL) == 0ULL) {
            a_exp >>= 1;
            t <<= 1;
        }
        /* compute p^t, stop if exceeds n */
        uint64_t p = primes[i];
        uint64_t q = 1;
        bool over = false;
        for (uint64_t j = 0; j < t; ++j) {
            if (q > n / p) {
                over = true;
                break;
            }
            q *= p;
        }
        if (!over && q <= n) {
            forbid[fcnt++] = q;
        }
    }

    /* total sum of 1..n */
    seq_t total = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;

    /* inclusion–exclusion over forbidden divisors */
    seq_t excl = 0;
    size_t subsets = 1ULL << fcnt;
    for (size_t mask = 1; mask < subsets; ++mask) {
        idx_t d = 1;
        for (size_t j = 0; j < fcnt; ++j) {
            if (mask & (1ULL << j)) {
                /* guard against overflow beyond n */
                if (d > n / forbid[j]) {
                    d = n + 1;          /* ensures d > n */
                    break;
                }
                d *= forbid[j];
            }
        }
        if (d > n) {
            continue;
        }
        idx_t m = n / d;
        seq_t sum_d = (seq_t)d * (seq_t)m * ((seq_t)m + (seq_t)1) / (seq_t)2;

        /* sign: + for odd subset size, - for even */
        if (__builtin_popcountll(mask) & 1) {
            excl += sum_d;
        } else {
            excl -= sum_d;
        }
    }

    return total - excl;
}
