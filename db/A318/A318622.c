/*
 * A318622  Number of irreducible factors in the factorization of the n-th
 *          cyclotomic polynomial over GF(2) (counted with multiplicity).
 * https://oeis.org/A318622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1 a(2^s * m) = 2^(s-1) * φ(m)/ord_m(2) (m odd,
 *            s≥0) where φ = Euler's totient and ord_m(2) is the
 *            multiplicative order of 2 mod m. For odd n (s=0) this reduces to
 *            a(n)=φ(n)/ord_n(2), the number of irreducible factors of Φ_n(x)
 *            over GF(2)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define PRIME_MAX 20000   /* primes up to this limit are sufficient for n ≤ 10000 */

static int  primes_composite[PRIME_MAX + 1];
static uint64_t primes[PRIME_MAX];
static size_t prime_cnt = 0;
static int primes_inited = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = 1;
    memset(primes_composite, 0, sizeof(primes_composite));
    for (uint64_t i = 2; i <= PRIME_MAX; ++i) {
        if (!primes_composite[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= PRIME_MAX) {
                for (uint64_t j = i * i; j <= PRIME_MAX; j += i)
                    primes_composite[j] = 1;
            }
        }
    }
}

/* modular exponentiation: 2^exp mod mod, using 128-bit intermediate to avoid overflow  */
static inline uint64_t mod_pow_uint64(uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t base   = 2 % mod;
    while (exp) {
        if (exp & 1) result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* Euler's totient function for unsigned 64-bit integers */
static uint64_t phi_uint64(uint64_t n)
{
    uint64_t res = n;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            res -= res / p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) res -= res / m;
    return res;
}

/* multiplicative order of 2 modulo m (m odd) */
static uint64_t multiplicative_order_mod2(uint64_t m)
{
    if (m == 1) return 1;
    uint64_t phi = phi_uint64(m);
    uint64_t order = phi;
    uint64_t tmp = phi;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            while (order % p == 0 &&
                   mod_pow_uint64(order / p, m) == 1)
                order /= p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        uint64_t p = tmp;
        while (order % p == 0 &&
               mod_pow_uint64(order / p, m) == 1)
            order /= p;
    }
    return order;
}

/* value of a(n) for odd n */
static seq_t a_odd(uint64_t m)
{
    if (m == 1) return (seq_t)1;
    uint64_t phi   = phi_uint64(m);
    uint64_t order = multiplicative_order_mod2(m);
    return (seq_t)(phi / order);
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    init_primes();           /* initialise prime list only once */

    uint64_t u = (uint64_t)n;
    uint64_t s = 0;          /* exponent of 2 in n */
    while ((u & 1) == 0) {
        ++s;
        u >>= 1;
    }
    uint64_t m = u;          /* odd part of n */

    seq_t base = a_odd(m);   /* a(n) for the odd part */

    if (s == 0) return base;

    /* multiply by 2^(s-1) */
    return (seq_t)((base) << (s - 1));
}
