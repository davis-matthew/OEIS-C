/*
 * A381498  a(n) = sum of numbers k <= n that have the same squarefree kernel
 *          as n.
 * https://oeis.org/A381498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = sum of all numbers k <= n that have the same squarefree
 *            kernel as n
 * Range:     u8 n <= 71, u16 n <= 4799, u32 n <= 9091, u64 n <= 9091,
 *            u128 n <= 9091, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Recursive helper that enumerates all products of the given primes
   with exponents >= 0, up to a given limit. For each product u it adds
   rad * u to the running sum. */
static void dfs(const uint64_t *primes, size_t prime_cnt, size_t idx,
                uint64_t current, uint64_t limit, uint64_t rad, seq_t *sum)
{
    if (idx == prime_cnt) {
        seq_t term = (seq_t)rad * (seq_t)current;
        *sum += term;
        return;
    }

    uint64_t p = primes[idx];
    uint64_t val = current;
    /* e = 0 case and all higher exponent cases */
    while (true) {
        dfs(primes, prime_cnt, idx + 1, val, limit, rad, sum);
        if (val > limit / p) break;
        val *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t N = n;
    if (N == 0) return 0;          /* behaviour below offset is irrelevant */

    /* Factor N and build its square‑free kernel (rad) and the list of primes. */
    uint64_t rad = 1;
    uint64_t primes[64];
    size_t prime_cnt = 0;

    uint64_t temp = N;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            primes[prime_cnt++] = p;
            rad *= p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {                  /* last prime factor > sqrt(N) */
        primes[prime_cnt++] = temp;
        rad *= temp;
    }

    /* All numbers with the same kernel are rad * u, where u is made
       only from the primes above and 1 <= u <= N / rad. */
    uint64_t limit = N / rad;
    seq_t sum = 0;
    dfs(primes, prime_cnt, 0, 1, limit, rad, &sum);

    return sum;
}
