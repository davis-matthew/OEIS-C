/*
 * A263042  a(n) = Sum_{i >= 1} d_i(n) * prime(i) where d_i(n) is the i-th
 *          digit of n in base 10, and prime(i) is the i-th prime.
 * https://oeis.org/A263042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i>=1} d_i(n) * prime(i) where d_i(n) is the i‑th
 *            digit of n from the right (d_1 = units digit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* First 25 primes – more than enough for 64‑bit indices (max 20 digits). */
static const uint32_t primes[25] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u
};

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t   digit;
    unsigned idx = 0;          /* position -> primes[idx] */

    while (n != 0) {
        digit = n % 10;        /* current rightmost digit */
        result += (seq_t)digit * (seq_t)primes[idx];
        n     /= 10;
        ++idx;
    }
    return result;
}
