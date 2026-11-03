/*
 * A305426  Number of proper divisors of n of the form 2^k - 1 for k >= 1.
 * https://oeis.org/A305426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of proper divisors of n that are of the form 2^k -
 *            1 (k ≥ 1)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9841 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Recursive helper to enumerate all divisors from the prime factorization
 * and count those that are Mersenne numbers. */
static void count_mersenne_rec(const uint64_t *primes, const int *exps,
                               int idx, int total, uint64_t current,
                               uint64_t n, seq_t *cnt)
{
    if (idx == total) {
        /* Exclude the divisor equal to n itself (proper divisor check). */
        if (current != n) {
            /* A number d is 2^k - 1 iff d+1 is a power of two.
             * (d+1) & d == 0    <=>  d+1 is a power of two. */
            if (((current + 1ULL) & current) == 0ULL) {
                (*cnt)++;
            }
        }
        return;
    }

    uint64_t val = current;
    for (int e = 0; e <= exps[idx]; ++e) {
        count_mersenne_rec(primes, exps, idx + 1, total, val, n, cnt);
        val *= primes[idx];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Treat n as an unsigned 64-bit integer for factorization. */
    uint64_t n64 = (uint64_t)n;

    /* Factorize n. */
    uint64_t primes[20];
    int     exps[20];
    int     num = 0;

    uint64_t temp = n64;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[num] = p;
            exps[num]   = e;
            num++;
        }
    }
    if (temp > 1) {               /* Remaining prime factor. */
        primes[num] = temp;
        exps[num]   = 1;
        num++;
    }

    seq_t count = 0;
    /* Enumerate all divisors using the recursive helper. */
    count_mersenne_rec(primes, exps, 0, num, 1ULL, n64, &count);

    return count;
}
