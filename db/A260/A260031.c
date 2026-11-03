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
 * Validated: standard, pass, 100 terms through n = 9889 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return m^n mod 12, where m is coprime with 6 and we only need parity of n */
static unsigned int coprime_m_pow_mod12(uint64_t n, unsigned int m_mod12) {
    return ( (n & 1ULL) ? m_mod12 : 1 );
}

/* Return 2^p mod 12, with p >= 0 */
static unsigned int pow2mod12(unsigned int p) {
    if (p == 0) return 1;
    if (p == 1) return 2;
    /* p >= 2: cycle 4,8 */
    return (p & 1) ? 8 : 4;
}

/* Return 3^q mod 12, with q >= 0 */
static unsigned int pow3mod12(unsigned int q) {
    if (q == 0) return 1;
    return (q & 1) ? 3 : 9;
}

static unsigned int v3_of(uint64_t x) {
    unsigned int c = 0;
    while (x % 3ULL == 0) { c++; x /= 3ULL; }
    return c;
}

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
