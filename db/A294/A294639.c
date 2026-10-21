/*
 * A294639  a(n) = least prime p such that n divides p + prime(n).
 * https://oeis.org/A294639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least prime p such that n divides (p + prime(n))
 * Range:     u8 n <= 52, u16 n <= 1818, u32 n <= 4831, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(1) = 2, a(2) = 3, a(3) = 7, etc.) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

/* static storage for prime numbers */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t   prime_limit = 0;

/* ensure that at least n primes are available in the static array */
static void ensure_primes_for_n(idx_t n) {
    if (prime_cnt >= n) return;           /* already enough */

    idx_t lim = prime_limit > 0 ? prime_limit * 2 : 100;
    if (lim < 50) lim = 100;

    while (1) {
        size_t cnt;
        uint64_t *tmp;

        build_primes_upto(lim, &tmp, &cnt);

        if (cnt >= n) {
            if (primes) free(primes);
            primes = tmp;
            prime_cnt = cnt;
            prime_limit = lim;
            return;
        }

        free(tmp);
        lim *= 2;
    }
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* obtain prime(n) */
    ensure_primes_for_n(n);
    seq_t prime_n = (seq_t)primes[n-1];

    /* compute r = (-prime_n) mod n, 0 <= r < n */
    seq_t mod_n = (seq_t)n;
    seq_t r = mod_n - (prime_n % mod_n);
    if (r == mod_n) r = (seq_t)0;         /* when prime_n % n == 0 */

    /* find the smallest integer >= 2 with value ≡ r (mod n) */
    seq_t p = r;
    if (p < (seq_t)2) {
        /* add enough multiples of n to reach at least 2 */
        uint64_t p0 = (uint64_t)p;
        uint64_t m   = ((2ull - p0) + (uint64_t)n - 1) / (uint64_t)n;
        p0 += m * (uint64_t)n;
        p   = (seq_t)p0;
    }

    /* search forward until a prime is found */
    while (!is_prime_big(p)) {
        p += mod_n;
    }

    return p;
}
