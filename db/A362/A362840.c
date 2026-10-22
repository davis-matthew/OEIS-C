/*
 * A362840  a(n) is the smallest number x between 1 and n-1 for which the
 *          number 1/x achieves the longest cycle of repeating digits in its
 *          expansion in base n.
 * https://oeis.org/A362840
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A362840:
 * a(n) = smallest x (1 < x < n) for which the base‑n expansion of 1/x
 * has the longest repeating cycle among all 1/x with 1 < x < n.
 *
 * The cycle length equals the multiplicative order of n modulo x
 * (order 0 if 1/x terminates in base n).
 * This implementation searches all coprime x and picks the one with
 * the maximum order, breaking ties by the smallest x.
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"

/* ---------- static data ---------- */
static int initialized = 0;
#define SPF_MAX 10000
static uint16_t spf[SPF_MAX + 1];
static uint32_t primes[SPF_MAX / 10];
static size_t prime_cnt;

/* ---------- helpers ---------- */
static void init(void)
{
    if (initialized) return;
    /* smallest prime factor sieve up to SPF_MAX */
    for (int i = 0; i <= SPF_MAX; ++i) spf[i] = 0;
    prime_cnt = 0;
    for (int i = 2; i <= SPF_MAX; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint16_t)i;
            primes[prime_cnt++] = (uint32_t)i;
            if ((int64_t)i * i <= SPF_MAX) {
                for (int j = i * i; j <= SPF_MAX; j += i)
                    if (spf[j] == 0) spf[j] = (uint16_t)i;
            }
        }
    }
    initialized = 1;
}

/* Euclidean gcd */

/* modular exponentiation (base^exp mod mod) */
static uint64_t powmod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t r = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL) r = (r * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return r;
}

/* collect unique prime factors of n (n <= SPF_MAX) */
static void get_unique_prime_factors(uint64_t n, uint64_t *fac, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint64_t p = spf[(int)n];
        fac[(*cnt)++] = p;
        do { n /= p; } while (n % p == 0);
    }
}

/* Euler's totient for n (n <= SPF_MAX) */
static uint64_t compute_phi(uint64_t n)
{
    uint64_t result = n;
    uint64_t tmp = n;
    while (tmp > 1) {
        uint64_t p = spf[(int)tmp];
        while (tmp % p == 0) tmp /= p;
        result = result / p * (p - 1);
    }
    return result;
}

/* multiplicative order of base modulo mod (gcd(base,mod)=1) */
static uint64_t multiplicative_order(uint64_t base, uint64_t mod)
{
    uint64_t phi = compute_phi(mod);
    uint64_t order = phi;

    /* trial division by prime factors of phi */
    uint64_t fac[32];
    size_t fcnt;
    get_unique_prime_factors(phi, fac, &fcnt);

    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = fac[i];
        while (order % p == 0 && powmod_u64(base, order / p, mod) == 1)
            order /= p;
    }
    return order;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    init();
    if (n < 3) return 0;                 /* undefined below offset */

    uint64_t best_order = 0;
    uint64_t best_x     = n;             /* any valid x is smaller */

    for (uint64_t x = 2; x < n; ++x) {
        if (gcd_uint64(x, n) != 1) continue;  /* terminating expansion */
        uint64_t ord = multiplicative_order(n, x);
        if (ord > best_order || (ord == best_order && x < best_x)) {
            best_order = ord;
            best_x     = x;
        }
    }
    return (seq_t)best_x;
}
