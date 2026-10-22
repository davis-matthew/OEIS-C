/*
 * A375777  Numbers k such that k - rad(k) - 1 is prime, where rad(k) is
 *          A007947(k).
 * https://oeis.org/A375777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   _smallest_prime_factor, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A375777 – Number k such that k – rad(k) – 1 is prime. */
/* The implementation searches successive integers, computing rad(k)
 * by repeated division by smallest prime factors (obtained via
 * _smallest_prime_factor).  Each candidate k is accepted when
 * k - rad(k) - 1 is prime (checked with is_prime_big).  The results
 * are cached so that calls in arbitrary order are efficient. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_k = 1; /* last integer examined */

    if (n == 0) return 0; /* undefined for n < 1, but avoid UB */

    /* grow cache if needed */
    while (cache_len < n) {
        idx_t k = ++last_k;            /* test next integer */
        idx_t m = k;
        seq_t rad = 1;

        /* compute rad(k) as product of distinct prime factors */
        while (m > 1) {
            uint64_t p = _smallest_prime_factor(m); /* p divides m */
            rad *= (seq_t)p;
            while (m % p == 0) m /= p;
        }

        /* from k - rad(k) - 1 must be prime */
        if ((seq_t)k <= rad + 1)          /* diff < 2, cannot be prime */
            continue;

        seq_t diff = (seq_t)k - rad - 1;  /* k > rad + 1 guaranteed */
        if (is_prime_big(diff)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
