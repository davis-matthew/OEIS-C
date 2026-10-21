/*
 * A260031  Final nonzero digit of n^n in base 12.
 * https://oeis.org/A260031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( n^n / 12^k ) mod 12 where k = min( n * v2(n) / 2 , n *
 *            v3(n) ), with v2(n) = exponent of 2 in n, v3(n) = exponent of 3
 *            in n. The result is the final non‑zero digit of n^n in base 12
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   coprime_m_pow_mod12, pow2mod12, pow3mod12, v3_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/coprime_m_pow_mod12.h"
#include "../utils/pow2mod12.h"
#include "../utils/pow3mod12.h"
#include "../utils/v3_of.h"

static unsigned int v2_of(uint64_t x) {
    unsigned int c = 0;
    while ((x & 1ULL) == 0) { c++; x >>= 1; }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return 1; /* not needed for n >= 1, but guard */
    /* compute exponents of 2 and 3 */
    unsigned int an = v2_of(n);
    unsigned int bn = v3_of(n);

    /* k = min( (an*n)/2 , bn*n ) */
    unsigned long long k1 = (unsigned long long)an * n / 2ULL;
    unsigned long long k2 = (unsigned long long)bn * n;
    unsigned long long k  = (k1 < k2) ? k1 : k2;

    unsigned long long p = (unsigned long long)an * n - 2ULL * k;
    unsigned long long q = (unsigned long long)bn * n - k;

    /* compute m = n / (2^an * 3^bn) modulo 12 */
    unsigned long long temp = n;
    while ((temp & 1ULL) == 0) temp >>= 1;
    while (temp % 3ULL == 0) temp /= 3ULL;
    unsigned int m_mod12 = (unsigned int)(temp % 12ULL);

    unsigned int r = pow2mod12((unsigned int)p);
    r = (r * pow3mod12((unsigned int)q)) % 12U;
    r = (r * coprime_m_pow_mod12(n, m_mod12)) % 12U;

    return (seq_t)r;
}
