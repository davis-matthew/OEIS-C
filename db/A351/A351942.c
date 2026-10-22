/*
 * A351942  Arithmetic derivative of A181819(n), where A181819(n) = Product
 *          prime(e(i)) when n = Product prime(i)^e(i).
 * https://oeis.org/A351942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9938, u16 n <= 9938, u32 n <= 9938, u64 n <= 9938,
 *            u128 n <= 9938, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A003415(A181819(n))
 *   where A181819(n) = ∏ prime(e_i)  for n = ∏ prime_i^e_i.
 *   A003415 is the arithmetic derivative.
 */

#include <stdint.h>

static const uint64_t prime_of_exp[65] = {
    0ULL, /* unused 0‑th element */
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
    127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
    179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL,
    233ULL, 239ULL, 241ULL, 251ULL, 257ULL, 263ULL, 269ULL, 271ULL, 277ULL, 281ULL,
    283ULL, 293ULL, 307ULL, 311ULL
};

seq_t a(idx_t n)
{
    if (n == 1) return 0; /* a(1) = 0 */

    uint64_t num = (uint64_t)n;
    uint8_t exp_counts[65] = { 0 };          /* exp_counts[e] = multiplicity of exponent e */

    uint64_t temp = num;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exp_counts[e]++;                  /* record one occurrence of exponent e */
        }
    }
    if (temp > 1) {
        /* remaining prime factor with exponent 1 */
        exp_counts[1]++;                    /* exponent 1 occurs once */
    }

    /* Compute m = ∏ prime_of_exp[e] ^ exp_counts[e] */
    seq_t m = 1;
    for (int e = 1; e <= 64; ++e) {
        uint8_t c = exp_counts[e];
        for (uint8_t i = 0; i < c; ++i) {
            m *= (seq_t)prime_of_exp[e];
        }
    }

    /* Compute arithmetic derivative: res = m * Σ (c_e / p_e) */
    seq_t result = 0;
    for (int e = 1; e <= 64; ++e) {
        uint8_t c = exp_counts[e];
        if (c == 0) continue;
        seq_t term = (m / (seq_t)prime_of_exp[e]) * (seq_t)c;
        result += term;
    }

    return result;
}
