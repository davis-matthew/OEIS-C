/*
 * A361760  a(n) = Product_{i=prime(n)..prime(n+1)-1} i.
 * https://oeis.org/A361760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=prime(n)}^{prime(n+1)-1} i
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 98,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

/* Ensure that primes[0..need-1] are defined (need >= 1). */
static void ensure_primes(idx_t need)
{
    while (prime_cnt < need) {
        uint64_t cand = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;
        while (!is_prime((idx_t)cand)) {
            ++cand;
        }
        if (prime_cnt >= prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 4;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) return;                 /* OOM – ignore for test harness */
            primes = tmp;
            prime_cap = new_cap;
        }
        primes[prime_cnt++] = cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;                 /* undefined at 0, return neutral element */

    ensure_primes(n + 1);                 /* need primes[n] which is the (n+1)-th prime */

    uint64_t start = primes[n - 1];
    uint64_t end   = primes[n] - 1;        /* inclusive upper bound */

    seq_t prod = 1;
    for (uint64_t i = start; i <= end; ++i)
        prod *= (seq_t)i;

    return prod;
}
