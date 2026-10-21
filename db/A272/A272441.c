/*
 * A272441  Primes with a prime number of binary digits.
 * https://oeis.org/A272441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 623, u32 n <= 8601, u64 n <= 8601,
 *            u128 n <= 8601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th prime whose binary representation has a prime number of bits */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* helper: primality test for 64‑bit values (deterministic) */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

/* helper: bit length of an unsigned 64‑bit integer */
static unsigned bitlen_uint64(uint64_t n)
{
    unsigned len = 0;
    while (n) {
        ++len;
        n >>= 1;
    }
    return len;
}

/* sequence function */
seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *primes = NULL;
    static size_t cap = 0;
    static idx_t len = 0;         /* number of primes stored */
    static uint64_t cur = 2;      /* next candidate to test */

    /* extend cache until we have at least n primes */
    while (len < n) {
        /* test primality of cur */
        if (is_prime_uint64(cur)) {
            unsigned bl = bitlen_uint64(cur);
            if (is_prime_uint64(bl)) {
                /* store prime into cache */
                if (len == (idx_t)cap) {
                    size_t newCap = (cap == 0) ? 4 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(primes, newCap * sizeof(seq_t));
                    if (!tmp) abort();
                    primes = tmp;
                    cap = newCap;
                }
                primes[(size_t)len] = (seq_t)cur;
                ++len;
            }
        }
        ++cur;
    }

    return primes[(size_t)(n - 1)];
}
