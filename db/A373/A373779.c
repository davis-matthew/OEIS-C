/*
 * A373779  a(n) is the least prime p such that the digit reversal of n*p is
 *          also prime, or -1 if no such prime exists.
 * https://oeis.org/A373779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) is the least prime p such that reverse(n·p) is prime, or -1
 *            otherwise. * The implementation memoizes results and respects
 *            the properties * a(10·n)=a(n) and a(n) = -1 when n is divisible
 *            by 3 or 11
 * Range:     i8 n <= 22, i16 n <= 961, i32 n <= 961, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   ensure_cache, is_prime_big, next_prime, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;         /* memoization array   */
    static size_t cache_cap = 0;        /* current capacity    */

    /* strip trailing zeros: a(10·m)=a(m) */
    while (n % 10 == 0) n /= 10;

    /* ensure cache big enough */
    ensure_cache(n, &cache, &cache_cap);
    if (cache[n] != 0)                 /* already computed */
        return cache[n];

    /* quick negative case */
    if (n % 3 == 0 || n % 11 == 0) {
        cache[n] = (seq_t)(-1);
        return (seq_t)(-1);
    }

    /* search primes */
    uint64_t p = 2;
    for (;;) {
        seq_t prod = (seq_t)n * (seq_t)p;
        seq_t rev  = reverse_decimal(prod);
        if (is_prime_big(rev)) {
            cache[n] = (seq_t)p;
            return (seq_t)p;
        }
        /* next prime candidate */
        p = next_prime(p + 1);
    }
}
