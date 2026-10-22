/*
 * A322965  Numerator of Sum_{d | n} 1/rad(d).
 * https://oeis.org/A322965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of \sum_{d|n} 1/rad(d) where rad(n) is the
 *            product of distinct prime factors of n
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the sequence A322965.
   The function finds the prime factorisation of n, builds the
   rational number f(n)=Π_{p^e||n} (p+e)/p,
   cancels all common 1‑factors with the denominator, and
   returns the resulting numerator as a seq_t. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* product of (p+e) for each prime divisor p^e of n */
    seq_t num = (seq_t)1;
    idx_t temp = n;
    idx_t i = 2;

    /* factor  n  using trial division */
    while (i <= temp / i) {
        if (temp % i == 0) {
            idx_t exp = 0;
            while (temp % i == 0) {
                temp /= i;
                exp++;
            }
            num *= (seq_t)(i + exp);
        }
        i++;
    }
    if (temp > 1) {            /* remaining prime factor */
        num *= (seq_t)(temp + 1);
    }

    /* cancel one factor of each prime divisor from the numerator */
    temp = n;
    i = 2;
    while (i <= temp / i) {
        if (temp % i == 0) {
            if (num % i == 0) {
                num /= (seq_t)i;
            }
            while (temp % i == 0) {
                temp /= i;
            }
        }
        i++;
    }
    if (temp > 1) {            /* last prime factor */
        if (num % temp == 0) {
            num /= (seq_t)temp;
        }
    }

    return num;
}
