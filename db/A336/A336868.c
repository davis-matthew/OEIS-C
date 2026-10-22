/*
 * A336868  Indicator function for numbers k such that k! has distinct prime
 *          multiplicities.
 * https://oeis.org/A336868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the prime exponents in n! are all distinct,
 *            otherwise 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache primes up to the largest n requested so far. */
    static uint64_t primes[20000];
    static size_t prime_count = 0;
    static uint64_t prime_limit = 1;   /* last prime generated */

    while (prime_limit < n) {
        prime_limit++;
        bool is_p = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > prime_limit)
                break;
            if (prime_limit % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p)
            primes[prime_count++] = prime_limit;
    }

    /* 0! and 1! are trivial; 2! has only one prime factor. */
    if (n < 2)
        return (seq_t)1;

    /* Array to detect duplicate exponents.
       For n ≤ 10000 the maximum exponent (for p = 2) is < 10000. */
    static bool seen[20001];
    int upper = (int)n;
    if (upper > 20000)
        upper = 20000;
    memset(seen, 0, (upper + 1) * sizeof(bool));

    for (size_t i = 0; i < prime_count && primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        uint64_t e = 0;
        uint64_t pk = p;
        while (pk <= n) {
            e += n / pk;
            if (pk > n / p)            /* stop to avoid overflow          */
                break;
            pk *= p;
        }
        if (seen[e])
            return (seq_t)0;           /* duplicate exponent found        */
        seen[e] = true;
    }
    return (seq_t)1;
}
