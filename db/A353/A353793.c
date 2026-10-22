/*
 * A353793  Multiplicative with a(p^e) = ((q-1)*p)^e, where q is the least
 *          prime larger than p.
 * https://oeis.org/A353793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e||n} ((nextprime(p)-1)*p)^e (sequence
 *            A353793)
 * Range:     u8 n <= 13, u16 n <= 161, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* -------- helper functions for small primes -------- */
static inline bool is_prime_small(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL)
        if (n % i == 0ULL) return false;
    return true;
}

static inline uint64_t next_prime_small(uint64_t p)
{
    uint64_t cand = p + 1ULL;
    if (cand <= 2ULL) return 2ULL;
    if (cand % 2ULL == 0ULL) cand++;
    for (;; cand += 2ULL)
        if (is_prime_small(cand)) return cand;
}

/* -------- sequence function -------- */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1; for n=0 just return 0 to be safe */
        return (seq_t)n;

    seq_t result = 1;
    idx_t m = n;

    while (m > 1) {
        /* find smallest prime factor of m */
        uint64_t p;
        if (m % 2ULL == 0ULL)
            p = 2ULL;
        else {
            p = 3ULL;
            for (; p * p <= (uint64_t)m; p += 2ULL) {
                if (m % p == 0ULL) break;
            }
            if (p * p > (uint64_t)m) p = m;   /* m itself is prime */
        }

        /* count exponent of p in m */
        idx_t e = 0;
        while (m % p == 0ULL) {
            m /= p;
            e++;
        }

        /* compute base = (nextprime(p)-1) * p  */
        uint64_t q = next_prime_small(p);
        seq_t base = (seq_t)(q - 1ULL) * (seq_t)p;

        /* compute base^e (e is small) */
        seq_t part = 1;
        for (idx_t i = 0; i < e; ++i)
            part *= base;

        result *= part;
    }
    return result;
}
