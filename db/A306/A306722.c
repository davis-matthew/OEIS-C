/*
 * A306722  Number of pairs of primes (p,q), p < q, which are a solution of
 *          the Diophantine equation (p-1)*(q-1) = (2n)^2.
 * https://oeis.org/A306722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of primes (p,q), p < q, such that
 *            (p-1)*(q-1) = (2n)^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Integer square root: floor(sqrt(x)) */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > x) bit >>= 2;
    for (; bit; bit >>= 2) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

/* Modular exponentiation (uint32_t) */
static uint32_t mod_pow_uint32(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint64_t result = 1;
    uint64_t b = base;
    while (exp) {
        if (exp & 1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint32_t)result;
}

/* Deterministic Miller–Rabin for n < 2^32.
   Bases 2, 7, 61 are sufficient. */
static bool is_prime32(uint32_t n)
{
    if (n < 2) return false;

    /* Small prime trial division */
    static const uint32_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    size_t i;
    for (i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint32_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Write n-1 = d * 2^s with d odd */
    uint32_t d = n - 1;
    uint32_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint32_t bases[] = { 2, 7, 61 };
    for (i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint32_t a = bases[i];
        if (a % n == 0) continue;
        uint32_t x = mod_pow_uint32(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        uint32_t r;
        for (r = 1; r < s; ++r) {
            x = (uint64_t)x * x % n;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 safely */

    uint64_t nn = (uint64_t)n;
    uint64_t M  = 4ULL * nn * nn;                 /* (2n)^2 = 4 n^2 */
    uint64_t sqrt_M = isqrt64(M);

    seq_t count = 0;
    for (uint64_t d = 1; d <= sqrt_M; ++d) {
        if (M % d != 0) continue;                /* d must divide M */
        uint64_t other = M / d;
        if (d >= other) continue;               /* ensures p < q */

        uint32_t p = (uint32_t)(d + 1);
        uint32_t q = (uint32_t)(other + 1);

        if (is_prime32(p) && is_prime32(q)) {
            ++count;
        }
    }
    return count;
}
