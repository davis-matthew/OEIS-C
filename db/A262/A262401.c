/*
 * A262401  In prime factorization of n: replace each factor with its largest
 *          decimal digit.
 * https://oeis.org/A262401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, mult
 * Formula:   a(n) = Π over prime factors p of n of ( largest decimal digit of
 *            p )^exp(p)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses trial division and no external factorization helper. */
#include <stdint.h>
#include <stddef.h>

static seq_t largest_digit_uint64(uint64_t x)
{
    seq_t d = 0;
    while (x > 0) {
        unsigned dig = (unsigned)(x % 10);
        if (dig > d) d = dig;
        x /= 10;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1). For n==1 the loop will not execute and
       the product of an empty set is 1, which is the correct value.      */
    uint64_t N = (uint64_t)n;
    seq_t result = 1;

    uint64_t i = 2;
    while (i * i <= N) {
        if (N % i == 0) {
            unsigned exp = 0;
            while (N % i == 0) {
                N /= i;
                ++exp;
            }
            seq_t digit = largest_digit_uint64(i);
            for (unsigned k = 0; k < exp; ++k) {
                result *= digit;
            }
        }
        ++i;
    }

    if (N > 1) { /* remaining prime factor */
        seq_t digit = largest_digit_uint64(N);
        result *= digit;
    }

    return result;
}
