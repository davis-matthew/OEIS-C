/*
 * A300289  a(n) is the smallest prime p such that the product of p and
 *          prime(n) contains only prime digits, or -1 if no such prime p
 *          exists.
 * https://oeis.org/A300289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = smallest prime p such that p*prime(n) contains only prime digits,
   otherwise -1.  (Indexing starts at 1.) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utilities for retrieving primes */
#include "../utils/get_prime.h"         /* uint32_t get_prime(idx_t k); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                      uint64_t **out, size_t *cnt); */

/* Return true iff all decimal digits of x are in {2,3,5,7} */
static bool prime_digits_only(seq_t x) {
    while (x > 0) {
        seq_t d = x % 10;
        if (d != 2 && d != 3 && d != 5 && d != 7)
            return false;
        x /= 10;
    }
    return true;
}

/* Cached list of primes up to a preset limit (enough for n <= 10 000). */
static uint64_t *prime_list   = NULL;
static size_t    prime_count  = 0;
static bool      primes_ready = false;

static void init_prime_list(void) {
    if (!primes_ready) {
        /* 20 million is more than sufficient for the problem constraints. */
        const idx_t LIMIT = 20000000;
        uint64_t *tmp = NULL;
        size_t cnt = 0;
        build_primes_upto(LIMIT, &tmp, &cnt);
        prime_list  = tmp;
        prime_count = cnt;
        primes_ready = true;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_prime_list();

    if (n == 0)
        return (seq_t)-1;   /* undefined for indices below offset */

    uint32_t pn = get_prime(n);           /* nth prime, 1‑based */
    seq_t prime_n = (seq_t)pn;

    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        seq_t prod = p * prime_n;
        if (prime_digits_only(prod))
            return p;
    }
    return (seq_t)-1;   /* no suitable prime found within the limit */
}
