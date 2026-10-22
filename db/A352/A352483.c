/*
 * A352483  Numerator of 1/d - 1/n = (n-d)/(n*d) where d is the number of
 *          divisors of n (A000005).
 * https://oeis.org/A352483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Numerator of 1/d(n) - 1/n = (n - d(n)) / (n * d(n)),
 *            reduced to lowest terms, where d(n) is the divisor count of n
 * Range:     u8 n <= 35, u16 n <= 6353, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute divisor count for a 64‑bit unsigned integer */
static uint64_t divisor_count_uint64(uint64_t x)
{
    uint64_t cnt = 1;
    uint64_t exp, p;
    if (x == 0) return 0;            /* only for completeness */
    /* factor 2 separately */
    exp = 0;
    while ((x & 1ULL) == 0) {
        x >>= 1;
        exp++;
    }
    if (exp > 0) cnt *= (exp + 1);

    /* odd primes */
    for (p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            cnt *= (exp + 1);
        }
    }
    if (x > 1) cnt *= 2;   /* remaining prime factor */
    return cnt;
}

/* Euclidean GCD for seq_t (works for built‑in integer types and
   for arbitrary‑precision integer classes that overload the arithmetic
   and modulo operators). */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Core formula:  a(n) = (n - d(n)) / (n * d(n))  reduced */
    if (n == 0) return 0; /* guard for safety, though n >= 1 in OEIS */

    uint64_t d_u = divisor_count_uint64((uint64_t)n);   /* d(n) */
    seq_t N = (seq_t)n;
    seq_t D = (seq_t)d_u;
    seq_t num = N - D;                     /* n - d(n) */
    seq_t denom = N * D;                   /* n * d(n) */
    seq_t g = gcd_seq(num, denom);         /* reduce fraction */
    return num / g;                        /* numerator of reduced fraction */
}
