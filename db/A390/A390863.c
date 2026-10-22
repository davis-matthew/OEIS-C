/*
 * A390863  The number of integers k from 1 to n such that gcd(n, k) is an
 *          infinitary divisor of n.
 * https://oeis.org/A390863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A390863: a(n) multiplicative with a(p^e) = 1 + (p-1)*sum_{k=1..e}[ (k & e)==k ]*p^(k-1) */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n = 1 the empty product yields 1, which is correct. */
    if (n == 0)
        return 0;

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* Handle factor 2 separately. */
    int exp2 = 0;
    while ((temp & 1ULL) == 0ULL) {
        ++exp2;
        temp >>= 1;
    }
    if (exp2 > 0) {
        uint64_t p = 2ULL;
        seq_t sum = 0;
        seq_t pow = 1;                 /* p^(k-1) */
        for (int k = 1; k <= exp2; ++k) {
            if (((uint64_t)k & exp2) == (uint64_t)k)
                sum += pow;
            pow = pow * (seq_t)p;      /* next power of p */
        }
        seq_t part = 1 + ((seq_t)(p - 1)) * sum;
        result *= part;
    }

    /* Odd prime factors. */
    uint64_t i = 3;
    while (i * i <= temp) {
        if (temp % i == 0) {
            int exp = 0;
            while (temp % i == 0) {
                ++exp;
                temp /= i;
            }
            uint64_t p = i;
            seq_t sum = 0;
            seq_t pow = 1;
            for (int k = 1; k <= exp; ++k) {
                if (((uint64_t)k & exp) == (uint64_t)k)
                    sum += pow;
                pow = pow * (seq_t)p;
            }
            seq_t part = 1 + ((seq_t)(p - 1)) * sum;
            result *= part;
        }
        i += 2;
    }

    /* Remaining prime > sqrt(n). */
    if (temp > 1) {
        uint64_t p = temp;
        int exp = 1;                  /* single exponent */
        seq_t sum = 0;
        seq_t pow = 1;
        for (int k = 1; k <= exp; ++k) {
            if (((uint64_t)k & exp) == (uint64_t)k)
                sum += pow;
            pow = pow * (seq_t)p;
        }
        seq_t part = 1 + ((seq_t)(p - 1)) * sum;
        result *= part;
    }

    return result;
}
