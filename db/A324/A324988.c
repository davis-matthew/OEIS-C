/*
 * A324988  Palindromes whose number of divisors is palindromic.
 * https://oeis.org/A324988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th palindrome m such that τ(m) is also a palindrome
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_palindrome_base, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

/* Return the n-th palindrome whose number of divisors is also a palindrome. */
seq_t a(idx_t n) {
    static seq_t *cache      = NULL;   /* stored qualifying palindromes            */
    static size_t cache_len  = 0;      /* number of terms already found           */
    static size_t cache_cap  = 0;      /* current allocation capacity             */
    static uint64_t last_checked = 0;  /* highest integer examined so far         */

    /* If we already know the answer, return it immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate palindromes until we have at least n terms */
    while (cache_len < (size_t)n) {
        ++last_checked;
        uint64_t m = last_checked;

        /* Check palindrome property (base 10) */
        if (is_palindrome_base((seq_t)m, 10)) {
            /* Compute the divisor count τ(m) */
            uint64_t tau = num_divisors(m);
            /* τ(m) must also be a palindrome */
            if (is_palindrome_base((seq_t)tau, 10)) {
                /* Ensure sufficient capacity for the cache */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_len++] = (seq_t)m;
            }
        }
    }

    return cache[n - 1];
}
