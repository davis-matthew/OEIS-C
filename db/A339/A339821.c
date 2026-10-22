/*
 * A339821  a(n) = phi(A019565(2n)), where phi is Euler totient function.
 * https://oeis.org/A339821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = product of (prime_e - 1) for every set bit e (e ≥ 2) of
 *            4n, where prime_e is the e‑th prime (1‑indexed). Equivalently
 *            a(n) = φ(A019565(2n))
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* First 62 primes.  prime_e is primes[e-1].  Index e≥2 uses primes[e-1]. */
static const uint64_t primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
    127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
    179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL,
    233ULL, 239ULL, 241ULL, 251ULL, 257ULL, 263ULL, 269ULL, 271ULL, 277ULL, 281ULL,
    283ULL, 293ULL, 307ULL, 311ULL, 313ULL
};

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    /* 4n as an unsigned 64‑bit value */
    uint64_t bits = (uint64_t)n << 2;
    int e = 0;

    while (bits) {
        if (bits & 1ULL) {
            if (e >= 2) {
                result *= (seq_t)(primes[e - 1] - 1ULL);
            }
        }
        bits >>= 1;
        ++e;
    }
    return result;
}
