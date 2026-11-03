/*
 * A384013  Primes with at least two identical leading digits.
 * https://oeis.org/A384013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime, mem, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes whose first two decimal digits are equal. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"
#include "../utils/num_digits0.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* The cache holds the sequence values found so far. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* The next integer to test for primality. */
static idx_t current = 11;

/* Append a new term to the cache, expanding it if necessary. */
static void push_back(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 32 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but avoid crash */

    /* Find terms until we have at least n of them. */
    while (cache_len < n) {
        if (is_prime(current)) {
            idx_t digits = (idx_t)num_digits0(current);
            if (digits >= 2) {
                seq_t pow10_val = pow10(digits - 2);   /* 10^(digits-2) */
                idx_t divisor = (idx_t)pow10_val;
                if (divisor != 0) {
                    idx_t first_two = current / divisor;   /* two leading digits */
                    if ((first_two / 10) == (first_two % 10)) {
                        push_back((seq_t)current);
                    }
                }
            }
        }
        ++current;
    }

    return cache[n - 1];
}
