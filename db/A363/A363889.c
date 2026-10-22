/*
 * A363889  Sum of divisors of 3*n-2 of form 3*k+1.
 * https://oeis.org/A363889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | (3*n-2), d mod 3 = 1} d
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper to recursively enumerate all divisors of m built from its prime
   factorisation and add those that are ≡ 1 (mod 3). */
static void dfs(size_t idx, seq_t cur, seq_t *sum,
                const uint64_t *primes, const int *exps, size_t num)
{
    if (idx == num) {
        if (cur % 3 == 1) {
            *sum += cur;
        }
        return;
    }
    uint64_t p = primes[idx];
    seq_t mult = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        dfs(idx + 1, mult, sum, primes, exps, num);
        mult *= (seq_t)p;   /* next power of p */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute m = 3*n - 2 */
    uint64_t m = 3ULL * (uint64_t)n - 2ULL;

    /* Factorise m into prime powers */
    uint64_t primes[64];
    int exps[64];
    size_t cnt = 0;
    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2)) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Enumerate and accumulate the desired divisors */
    seq_t sum = 0;
    dfs(0, (seq_t)1, &sum, primes, exps, cnt);
    return sum;
}
