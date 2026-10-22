/*
 * A333955  Numbers k with digits in nondecreasing order and each digit
 *          greater than 1 such that the iterated product of digits of k is a
 *          prime.
 * https://oeis.org/A333955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A333955 computed by enumerating nondecreasing‑digit numbers
 *            (digits 2–9) and selecting those whose iterated digit product is
 *            prime
 * Range:     u8 n <= 10, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_prod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_prod_uint64.h"

/* trial division primality test for 64‑bit values (sufficient for the digit products). */
static inline bool is_prime_uint64(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (uint64_t i = 3; i * i <= v; i += 2)
        if (v % i == 0) return false;
    return true;
}

/* returns true iff the iterated product of digits of v is a prime. */
static bool iter_prime(uint64_t v)
{
    uint64_t prod = digit_prod_uint64(v);
    while (true) {
        if (prod <= 1) return false;
        if (is_prime_uint64(prod)) return true;
        uint64_t nxt = digit_prod_uint64(prod);
        if (nxt == prod) return false;   /* cycle: never reach a prime */
        prod = nxt;
    }
}

/* recursive generation of all length‑len nondecreasing numbers with digits 2–9.
   When a number passes the prime test it is stored in cache. */
static void gen_len(idx_t len, int start_digit, uint64_t cur,
                    seq_t **cache_ptr, size_t *built_ptr, size_t *cap_ptr)
{
    if (len == 0) {
        if (iter_prime(cur)) {
            if (*built_ptr == *cap_ptr) {
                size_t new_cap = (*cap_ptr == 0) ? 128 : (*cap_ptr) * 2;
                seq_t *tmp = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
                *cache_ptr = tmp;
                *cap_ptr = new_cap;
            }
            (*cache_ptr)[*built_ptr] = (seq_t)cur;
            (*built_ptr)++;
        }
        return;
    }
    for (int d = start_digit; d <= 9; ++d)
        gen_len(len - 1, d, cur * 10 + d, cache_ptr, built_ptr, cap_ptr);
}

/* public interface: a(idx_t n) returns the nth term of A333955 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n==0 – return 0 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t built = 0;
    static idx_t next_len = 1;          /* next digit length to generate */

    while (built < n) {
        gen_len(next_len, 2, 0, &cache, &built, &cache_cap);
        next_len++;
    }
    return cache[n - 1];
}
