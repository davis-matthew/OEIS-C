/*
 * A293227  a(n) is the number of proper divisors of n that are squarefree.
 * https://oeis.org/A293227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^ω(n) - 1_{squarefree}(n)
 * Range:     u8 n <= 7473, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9208 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the distinct prime factor count and
 * square‑free test. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Compute 2^exp for any exponent that may not fit in the bit width of seq_t. */
static seq_t pow2(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)2;
    return res;
}

/* Test whether x is square‑free.  Simple trial division works
   because numbers in the sequence are relatively small for the
   required range of n. */
static bool is_squarefree(seq_t x)
{
    for (idx_t d = 2; (seq_t)d * (seq_t)d <= x; ++d) {
        if (x % d == 0) {
            x /= d;
            if (x % d == 0) return false;
        }
    }
    return true;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    /* n >= 1 (offset is 1). */
    int omega = count_distinct_primes((uint64_t)n);          /* number of distinct primes in n */
    seq_t result = pow2((idx_t)omega);                     /* 2^omega(n) */
    if (is_squarefree(n))                                 /* mu(n)^2 = 1 if n squarefree */
        result -= 1;                                      /* subtract 1, else subtract 0 */
    return result;
}
