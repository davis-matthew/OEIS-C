/*
 * A401006  a(1)=1; for n >= 2, a(n) = n if n is prime; otherwise, a(n) is the
 *          decimal concatenation of n/p followed by p, where p is the largest
 *          prime factor of n.
 * https://oeis.org/A401006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, new
 * Formula:   a(n) = n if n is prime; otherwise a(n) = concat(n/p, p) where p
 *            is the largest prime factor of n (concatenation performed in
 *            base 10)
 * Range:     u8 n <= 32, u16 n <= 6665, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9453 (2026-11-02)
 * Helpers:   is_prime_seq, isqrt, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_seq.h"
#include "../utils/isqrt.h"
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Return the largest prime factor of n (in the range 1..UINT64_MAX).
   For n <= 1 return 1. */
static uint64_t largest_prime_factor_uint64(uint64_t n)
{
    if (n <= 1) return 1;
    if (is_prime_seq(n)) return n;
    uint64_t largest = 1;
    uint64_t r = isqrt(n);
    for (uint64_t f = 2; f <= r; ++f) {
        if (n % f == 0) {
            largest = f;
            while (n % f == 0) n /= f;
            r = isqrt(n);
        }
    }
    if (n > 1) largest = n;   /* the remaining factor is prime */
    return largest;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    uint64_t p = largest_prime_factor_uint64((uint64_t)n);
    if (p == n) {                    /* n is prime */
        return (seq_t)n;
    }
    seq_t m = (seq_t)(n / p);        /* n is composite, n = m * p */
    unsigned int d = (unsigned int)num_digits0(p);
    seq_t ten_pow = pow10((idx_t)d); /* 10^d as seq_t */
    return m * ten_pow + (seq_t)p;   /* decimal concatenation */
}
