/*
 * A243103  Product of numbers m with 2 <= m <= n whose prime divisors all
 *          divide n.
 * https://oeis.org/A243103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all m (1 <= m <= n) whose prime factors all
 *            divide n
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 29, u64 n <= 41, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive enumeration of all numbers composed of the given primes ≤ n.
   multiplies each such number into *prod. */
static void dfs(const uint64_t *primes, int k, int idx, uint64_t cur,
                seq_t *prod, uint64_t n)
{
    if (idx == k) {
        *prod = *prod * (seq_t)cur;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = cur;
    for (;;) {
        dfs(primes, k, idx + 1, val, prod, n);
        if (val > n / p) break;
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;

    const uint64_t N = (uint64_t)n;

    /* Factor N to obtain its distinct prime divisors. */
    uint64_t primes[20];
    int k = 0;
    uint64_t temp = N;

    if ((temp & 1ULL) == 0) {
        primes[k++] = 2;
        while ((temp & 1ULL) == 0) temp >>= 1;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            primes[k++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[k++] = temp;

    seq_t prod = (seq_t)1;
    dfs(primes, k, 0, 1, &prod, N);
    return prod;
}
