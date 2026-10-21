/*
 * A262433  Quater-imaginary representation of the Gaussian primes with an
 *          even imaginary part.
 * https://oeis.org/A262433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   eval_qi, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A262433: quater‑imaginary representations of Gaussian primes with an even
 * imaginary part.  The code enumerates all positive integers whose decimal
 * digits belong to {0,1,2,3}, interprets each as a base‑(2i) number,
 * tests whether the resulting Gaussian integer is a Gaussian prime and
 * has an even imaginary part, and returns the n‑th such number in
 * ascending order.  The result is returned as seq_t. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/eval_qi.h"

/* Determine whether a Gaussian integer a+bi is a prime in Z[i] and
 * has an even imaginary part.  It is assumed that bi is even. */
static inline bool is_gaussian_prime(int64_t a, int64_t b)
{
    if ((b & 1) != 0)               /* imaginary part is not even */
        return false;
    if (b == 0) {                   /* purely real case */
        if (a < 0) a = -a;
        if (a <= 1) return false;
        return (a & 3) == 3 && is_prime_small((uint64_t)a);
    }
    if (a == 0)                      /* purely imaginary is never prime */
        return false;
    uint64_t aa = (a < 0) ? (uint64_t)(-a) : (uint64_t)a;
    uint64_t bb = (b < 0) ? (uint64_t)(-b) : (uint64_t)b;
    uint64_t norm = aa * aa + bb * bb;
    return norm > 1 && is_prime_small(norm);
}

/* Return the nth member of this sequence (offset 1).  The function uses
 * memoisation; it may be called with arbitrary mix of n values. */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* next integer to inspect in base‑4 enumeration */
    static uint64_t next_base4 = 1;

    if (n == 0) return 0;              /* offset 1: behaviour below is irrelevant */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Obtain the next candidate number: decimal number whose digits
         * are the base‑4 digits of next_base4. */
        uint64_t m = next_base4;
        ++next_base4;

        uint8_t digits[32];
        int nd = 0;
        uint64_t u = m;
        while (u > 0) {
            digits[nd++] = u % 4;
            u /= 4;
        }

        /* Convert base‑4 digits to the corresponding decimal integer. */
        uint64_t dec = 0;
        for (int i = nd - 1; i >= 0; --i)
            dec = dec * 10 + digits[i];

        /* Evaluate the quater‑imaginary value */
        int64_t real, imag;
        eval_qi(dec, &real, &imag);

        /* Only keep numbers that represent a Gaussian prime with even
         * imaginary part. */
        if (is_gaussian_prime(real, imag)) {
            /* Expand cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)dec;
        }
    }

    return cache[n - 1];
}
