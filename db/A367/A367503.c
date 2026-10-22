/*
 * A367503  Sum of the final digits of the squarefree divisors of n.
 * https://oeis.org/A367503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Sum_{d|n} mu(d)^2 * (d mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the final digits of the squarefree divisors of n. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first term. */
    if (n == 0) {
        return (seq_t)0;        /* Undefined for 0; return 0 for safety. */
    }

    /* Extract distinct prime factors of n. */
    uint64_t num = (uint64_t)n;
    uint64_t primes[10];
    int k = 0;

    /* Small primes up to 100; enough to factor any n <= 10 000. */
    static const unsigned int small_primes[] = {
        2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
        31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u,
        71u, 73u, 79u, 83u, 89u, 97u
    };
    const int primes_cnt = (int)(sizeof(small_primes) / sizeof(small_primes[0]));

    for (int i = 0; i < primes_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > num) {
            break;
        }
        if (num % p == 0) {
            primes[k++] = p;
            do {
                num /= p;
            } while (num % p == 0);
        }
    }
    if (num > 1) {
        primes[k++] = num;           /* Remaining prime factor. */
    }

    /* Sum over all subsets of the distinct prime factors. */
    seq_t sum = (seq_t)0;
    const size_t total_masks = (size_t)1 << k;   /* 2^k subsets. */

    for (size_t mask = 0; mask < total_masks; ++mask) {
        seq_t prod = (seq_t)1;
        for (int i = 0; i < k; ++i) {
            if (mask & ((size_t)1 << i)) {
                seq_t digit = (seq_t)(primes[i] % 10);
                prod = (prod * digit) % (seq_t)10;
            }
        }
        sum += prod;
    }

    return sum;
}
