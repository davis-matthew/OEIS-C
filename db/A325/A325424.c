/*
 * A325424  Complement of A036668: numbers not of the form 2^i*3^j*k, i + j
 *          even, (k,6) = 1.
 * https://oeis.org/A325424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth positive integer whose exponents of 2 and 3 sum
 *            to an odd number (the complement of A036668)
 * Range:     u8 n <= 109, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to allow calls in any order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next_val = 2;   /* next integer to test */

    if (n == 0) return 0;  /* undefined for offset 1, but keep deterministic */

    /* grow cache as needed until we have n terms */
    while (cache_len < n) {
        seq_t v = (seq_t)next_val;
        idx_t parity = 0; /* parity of (exponent of 2 + exponent of 3) */

        /* remove all factors of 2 */
        while (v % 2 == 0) {
            v /= 2;
            parity ^= 1;
        }
        /* remove all factors of 3 */
        while (v % 3 == 0) {
            v /= 3;
            parity ^= 1;
        }

        /* if parity is odd, this number is part of the sequence */
        if (parity == 1) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_val;
        }
        ++next_val;
    }

    return cache[n - 1];
}
