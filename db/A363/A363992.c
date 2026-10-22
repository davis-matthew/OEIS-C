/*
 * A363992  The number of ways 2n can be expressed as the sum of an odd prime
 *          number and an odd nonprime, both of which are relatively prime to
 *          n.
 * https://oeis.org/A363992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations 2n = p + c with p odd prime, c
 *            odd nonprime, gcd(p,n)=1, gcd(c,n)=1
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static inline seq_t gcd_uint64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return (seq_t)a;
}

seq_t a(idx_t n) {
    /* Compute 2n in seq_t to avoid overflow for large seq_t types */
    /* But for n up to 10k we can safely use uint64_t for intermediate */
    uint64_t target = (uint64_t)2 * (uint64_t)n;  /* 2n */

    /* Handle trivial cases quickly */
    if (target < 3) return (seq_t)0;   /* smallest 2n with odd prime + odd nonprime is 2*2=4 */

    /* Sieve of Eratosthenes up to target to get primes and prime flags */
    size_t limit = (size_t)target;
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return (seq_t)0;  /* failure, return 0 */

    /* Initialize */
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (size_t i = 2; (size_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Build prime list */
    size_t primes_capacity = 0;
    size_t primes_count = 0;
    uint64_t *primes = NULL;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_count == primes_capacity) {
                size_t new_capacity = primes_capacity ? primes_capacity * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_capacity * sizeof(uint64_t));
                if (!tmp) {
                    free(is_prime);
                    free(primes);
                    return (seq_t)0;
                }
                primes = tmp;
                primes_capacity = new_capacity;
            }
            primes[primes_count++] = (uint64_t)i;
        }
    }

    /* Count valid representations */
    seq_t count = 0;
    for (size_t idx = 0; idx < primes_count; ++idx) {
        uint64_t p = primes[idx];
        if (p == 2) continue;          /* we need odd prime */
        if (p > target - 1) break;     /* p must be <= 2n-1 */

        uint64_t c = target - p;       /* odd c automatically (even-odd) */

        /* c must be an odd nonprime (including 1) */
        if (c == 0) continue;          /* safety, though impossible here */
        if (c > limit) continue;       /* shouldn't happen */

        if (c == 1 || !is_prime[c]) {  /* c is odd nonprime */
            if (gcd_uint64(p, (uint64_t)n) == 1 &&
                gcd_uint64(c, (uint64_t)n) == 1) {
                ++count;
            }
        }
    }

    free(is_prime);
    free(primes);
    return count;
}
