/*
 * A308828  Number of sequences that include all residues modulo n starting
 *          with x_0 = 0 and then x_i given recursively by x_{i+1} = a * x_i +
 *          c (mod n) where a and c are integers in the interval [0..n-1].
 * https://oeis.org/A308828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) * n / L * where L = rad(n) if n % 4 != 0, * L =
 *            rad(n) * 2 if n % 4 == 0 * rad(n) is the product of distinct
 *            prime factors of n * phi(n) is Euler's totient function
 * Range:     u8 n <= 48, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t _small_primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
    71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};
static const size_t _small_primes_cnt = 25;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, but guard */

    /* Compute Euler's phi(n) and rad(n) */
    idx_t phi = n;
    idx_t rad = 1;
    idx_t temp = n;

    for (size_t i = 0; i < _small_primes_cnt &&
                   (_small_primes[i] * (uint64_t)_small_primes[i] <= (uint64_t)temp);
         ++i) {
        uint64_t p = _small_primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            phi -= phi / p;
            rad *= p;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        phi -= phi / temp;
        rad *= temp;
    }

    /* Determine L */
    idx_t L = rad;
    if ((n & 3ULL) == 0) {     /* n divisible by 4 */
        L <<= 1;                /* multiply by 2 */
    }

    /* a(n) = phi(n) * n / L */
    seq_t result = ((seq_t)phi * (seq_t)n) / (seq_t)L;
    return result;
}
