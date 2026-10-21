/*
 * A259654  a(n) = prime(n)^prime(n) - prime(n)^n.
 * https://oeis.org/A259654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^prime(n) - prime(n)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   _prime_limit_for_idx, big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

static uint64_t *prime_cache = NULL;
static size_t prime_count = 0;

static void ensure_primes(idx_t n)
{
    if (prime_cache == NULL || prime_count < n) {
        idx_t lim = _prime_limit_for_idx(n);
        uint64_t *tmp = NULL;
        size_t cnt = 0;
        build_primes_upto(lim, &tmp, &cnt);
        if (prime_cache != NULL) free(prime_cache);
        prime_cache = tmp;
        prime_count = cnt;
    }
}

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first prime 2, etc. */
    if (n < 1) return (seq_t)0;      /* undefined for n <= 0, return 0 */

    ensure_primes(n);

    seq_t p = (seq_t)prime_cache[n - 1];
    seq_t pow_pp = big_pow(p, p);
    seq_t pow_pn = big_pow(p, (seq_t)n);
    return pow_pp - pow_pn;
}
