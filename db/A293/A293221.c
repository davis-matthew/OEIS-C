/*
 * A293221  a(n) = Product_{d|n, d<n} A019565(A289813(d)); a product obtained
 *          from the 1-digits present in ternary expansions of proper divisors
 *          of n.
 * https://oeis.org/A293221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Product_{d|n, d<n} A019565(A289813(d)); i.e. for each
 *            proper divisor d, multiply the prime(k+1) over every ternary
 *            digit position k of d whose digit is 1
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 179, u64 n <= 599,
 *            u128 n <= 1679, big any n
 * Validated: standard, pass, 101 terms through n = 6433 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A293221_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

/* A019565(A289813(d)) computed directly from the ternary digits of d */
static seq_t A293221_f(idx_t d)
{
    seq_t r = 1;
    unsigned k = 0;
    while (d != 0) {
        if (d % 3 == 1)
            r *= (seq_t)A293221_primes[k];
        d /= 3;
        k++;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t i;
    for (i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            idx_t j = n / i;
            if (i < n)
                result *= A293221_f(i);
            if (j != i && j < n)
                result *= A293221_f(j);
        }
    }
    return result;
}
