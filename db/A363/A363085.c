/*
 * A363085  Sum of the refactorable unitary divisors of n.
 * https://oeis.org/A363085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, tau(d)|d, gcd(d,n/d)=1} d
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t k = 0;

    uint64_t N = (uint64_t)n;
    if (N == 0) return 0;   /* undefined index, safe return */

    /* Factorization by trial division */
    if (N % 2 == 0) {
        uint64_t cnt = 0;
        while (N % 2 == 0) {
            N /= 2;
            cnt++;
        }
        primes[k] = 2;
        exps[k] = cnt;
        k++;
    }
    for (uint64_t p = 3; p * p <= N; p += 2) {
        if (N % p == 0) {
            uint64_t cnt = 0;
            while (N % p == 0) {
                N /= p;
                cnt++;
            }
            primes[k] = p;
            exps[k] = cnt;
            k++;
        }
    }
    if (N > 1) {
        primes[k] = N;
        exps[k] = 1;
        k++;
    }

    /* Enumerate unitary divisors (subsets of prime powers) */
    seq_t sum = 0;
    uint64_t total = 1ULL << k;          /* k <= number of distinct primes (<64) */
    for (uint64_t mask = 0; mask < total; ++mask) {
        seq_t d = 1;
        seq_t tau = 1;
        for (size_t i = 0; i < k; ++i) {
            if (mask & (1ULL << i)) {
                seq_t pow_val = pow_int((seq_t)primes[i], (idx_t)exps[i]);
                d = d * pow_val;
                tau = tau * (seq_t)(exps[i] + 1);
            }
        }
        if (d % tau == 0) {
            sum = sum + d;
        }
    }
    return sum;
}
