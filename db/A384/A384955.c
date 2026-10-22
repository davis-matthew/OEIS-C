/*
 * A384955  a(n) is the multinomial coefficient of the digits of n.
 * https://oeis.org/A384955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = multinomial coefficient of the decimal digits of n
 *            computed as a(n) = (sum_of_digits)! / ∏ (digit)! for the decimal
 *            representation of n This implementation avoids overflow by using
 *            prime‑factor exponent cancellation. It works for any seq_t that
 *            supports the usual arithmetic operators (+, *...
 * Range:     u8 n <= 46, u16 n <= 168, u32 n <= 698, u64 n <= 9998,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for size_t */

static const int primes_tbl[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47              /* all primes ≤ 50 (s ≤ 45 for n ≤ 10 000) */
};
#define NUM_PRIMES (sizeof(primes_tbl)/sizeof(primes_tbl[0]))

seq_t a(idx_t n)
{
    /* Extract decimal digits and compute their sum. */
    unsigned char digit[10];
    unsigned digit_cnt = 0;
    idx_t tmp = n;
    seq_t sum = 0;

    if (tmp == 0) {                 /* n = 0 ==> digits = {0} */
        digit[0] = 0;
        digit_cnt = 1;
    } else {
        while (tmp > 0) {
            unsigned d = tmp % 10;
            digit[digit_cnt++] = (unsigned char)d;
            sum += d;
            tmp /= 10;
        }
    }

    /* If the sum is zero, the multinomial coefficient is 1. */
    if (sum == 0)
        return (seq_t)1;

    /* Compute prime exponents of factorial(sum) and subtract those of
       factorial of each digit.  This uses Legendre's formula. */
    int exponents[NUM_PRIMES] = {0};

    for (size_t pi = 0; pi < NUM_PRIMES; ++pi) {
        int p = primes_tbl[pi];
        int power = p;
        /* exponent of p in sum! */
        while (power <= sum) {
            exponents[pi] += (int)(sum / power);
            power *= p;
        }
        /* subtract exponent of p in each digit! */
        for (unsigned i = 0; i < digit_cnt; ++i) {
            int d = digit[i];
            power = p;
            while (power <= d) {
                exponents[pi] -= (int)(d / power);
                power *= p;
            }
        }
    }

    /* Multiply primes raised to the computed exponents to obtain the
       result.  All multiplications are within seq_t. */
    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < NUM_PRIMES; ++pi) {
        int p   = primes_tbl[pi];
        int exp = exponents[pi];
        while (exp-- > 0)
            result *= (seq_t)p;
    }

    return result;
}
