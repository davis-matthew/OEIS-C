/*
 * A400259  Sum of digits when n' [the arithmetic derivative of n] is written
 *          in primorial base (A049345).
 * https://oeis.org/A400259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of digits of the arithmetic derivative of n written
 *            in primorial base (A049345)
 * Range:     u8 n <= 532, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle small n where the derivative is 0 */
    if (n <= 1) return 0;

    /* Compute arithmetic derivative n' = n * Σ e_i / p_i using prime factorisation */
    seq_t orig = (seq_t)n;          /* the original n as seq_t */
    seq_t derivative = 0;           /* result of n' */
    idx_t temp = n;                 /* mutable copy for factorisation */

    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            size_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            /* term = (n / p) * e  */
            derivative += (seq_t)(orig / (seq_t)p) * (seq_t)e;
        }
    }
    if (temp > 1)  /* remaining prime factor */
    {
        derivative += (seq_t)(orig / (seq_t)temp);
    }

    /* Sum the digits of derivative in primorial base */
    if (derivative == 0) return 0;

    /* First few primes – sufficient for n up to at least 10 000 (derivative < 2·10⁶) */
    static const uint32_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
        179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
        233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
        283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
        353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
        419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
        467, 479, 487, 491, 499, 503, 509, 521, 523, 541
    };
    size_t primes_len = sizeof(primes)/sizeof(primes[0]);

    seq_t sum = 0;
    seq_t m   = derivative;

    for (size_t i = 0; m != 0; ++i)
    {
        seq_t base   = (seq_t)primes[i];
        seq_t digit  = m % base;
        sum += digit;
        m    = m / base;
    }

    return sum;
}
