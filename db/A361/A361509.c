/*
 * A361509  a(n) = smallest Fibonacci number F(k) such that F(k) + F(n) is a
 *          prime, or -1 if no such F(k) exists.
 * https://oeis.org/A361509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, less
 * Formula:   a(n) = smallest Fibonacci F(k) such that F(k)+F(n) is prime, or
 *            -1 if none (indices: a(0) is the first term)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   fibonacci, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/fibonacci.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    seq_t fn = fibonacci(n);      /* F(n) */
    seq_t f_prev = 0;            /* F(0) */
    seq_t f_curr = 1;            /* F(1) */

    /* k = 0 */
    if (isPrimeGeneric(fn + f_prev))
        return f_prev;
    /* k = 1 */
    if (isPrimeGeneric(fn + f_curr))
        return f_curr;

    /* iterate k >= 2 */
    for (idx_t k = 2; k <= 1000000u; ++k) {
        seq_t f_next = f_prev + f_curr; /* F(k) */
        f_prev = f_curr;
        f_curr = f_next;
        if (isPrimeGeneric(fn + f_curr))
            return f_curr;
    }
    /* no such F(k) within our search bound */
    return (seq_t)-1;
}
