/*
 * A352834  Least k > 0 such that denominator( d(k*n)/(k*n) ) = n, or 0 if no
 *          such k exists, where d = A000005 is the number-of-divisors
 *          function.
 * https://oeis.org/A352834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that * denominator( d(k*n)/(k*n) ) = n,
 *            * equivalently gcd( d(k*n), k*n ) = k. * Here d(m) is the
 *            number-of-divisors function. Return 0 * if no such k exists.
 *            Bound: 0 < k ≤ 4*n. Complexity O(n·√n)
 * Range:     u8 n <= 9239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* sieve of Eratosthenes up to 20000 */
#define MAX_PRIME 20000
static idx_t primes[MAX_PRIME];   /* enough for all primes ≤ 20000 */
static size_t prime_cnt = 0;
static bool primed = false;

static void init_primes(void)
{
    if (primed) return;
    bool is_comp[MAX_PRIME + 1] = {0};
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                is_comp[j] = true;
        }
    }
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (!is_comp[i])
            primes[prime_cnt++] = (idx_t)i;
    primed = true;
}

/* Euclidean gcd on unsigned 64‑bit integers */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0u) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* divisor count of m (fits in uint64_t, uses primes up to 20000) */
static uint64_t divisor_count(uint64_t m)
{
    uint64_t d = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > m) break;
        uint64_t exp = 0;
        while (m % p == 0u) {
            ++exp;
            m /= p;
        }
        if (exp) d *= (exp + 1);
    }
    if (m > 1u) d *= 2;  /* remaining prime factor */
    return d;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    init_primes();

    /* upper bound for k */
    idx_t max_k = 4 * n;
    for (idx_t k = 1; k <= max_k; ++k) {
        uint64_t m = (uint64_t)k * (uint64_t)n;    /* fits in 64‑bit */
        uint64_t d = divisor_count(m);

        if (d % (uint64_t)k != 0u) continue;

        uint64_t d_div_k = d / (uint64_t)k;
        /* check coprimality with n */
        if (gcd64(d_div_k, (uint64_t)n) == 1u)
            return (seq_t)k;
    }
    return (seq_t)0;
}
