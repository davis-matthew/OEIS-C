/*
 * A328476  Convert the primorial base expansion of n into its prime product
 *          form, then subtract the largest primorial which divides that
 *          product: a(n) = A276151(A276086(n)).
 * https://oeis.org/A328476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276151(A276086(n)) Convert n to primorial base, form the
 *            product of primesᵈᵢ, and subtract the largest primorial dividing
 *            that product
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 416, u64 n <= 2308,
 *            u128 n <= 2559, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* lazily build a sufficiently large prime table once */
    static bool primes_ready = false;
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    if (!primes_ready) {
        const idx_t LIMIT = 200000;   /* more than enough for n up to ~10⁴  */
        build_primes_upto(LIMIT, &primes, &prime_cnt);
        primes_ready = true;
    }

    /* extract the primorial‑base digits of n */
    idx_t rem = n;
    size_t i = 0;
    uint64_t *digits = NULL;
    size_t digits_cap = 0, digits_len = 0;

    /* ensure at least one digit for n == 0 */
    while (rem > 0 || digits_len == 0) {
        if (digits_len >= digits_cap) {
            size_t new_cap = digits_cap ? digits_cap * 2 : 8;
            uint64_t *tmp = (uint64_t *)realloc(digits, new_cap * sizeof(uint64_t));
            if (!tmp) { free(digits); return 0; }
            digits = tmp;
            digits_cap = new_cap;
        }
        uint64_t base = primes[i];          /* next prime as radix */
        uint64_t d = rem % base;
        digits[digits_len++] = d;
        rem /= base;
        ++i;
    }

    /* build the product Π p_i^d_i */
    seq_t prod = 1;
    for (size_t k = 0; k < digits_len; ++k) {
        uint64_t di = digits[k];
        if (di == 0) continue;
        prod *= pow_seq_t((seq_t)primes[k], (idx_t)di);
    }

    /* count leading non‑zero digits to identify the largest primorial divisor */
    size_t leading = 0;
    for (size_t k = 0; k < digits_len; ++k) {
        if (digits[k] == 0) break;
        ++leading;
    }

    seq_t primorial = 1;
    for (size_t k = 0; k < leading; ++k) {
        primorial *= (seq_t)primes[k];
    }

    free(digits);
    return prod - primorial;
}
