/*
 * A352563  a(n) = number of modules with n elements over the ring of integers
 *          in the imaginary quadratic field of discriminant -7.
 * https://oeis.org/A352563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} local_factor(p, e) where local_factor(p,0) =
 *            1 and for e>0 – if p = 7 (ramified) local_factor = partition(e)
 *            – if p = 2 or (−7|p)=1 (split) local_factor = Σ_{k=0}^e
 *            partition(k)·partition(e−k) – otherwise (inert) local_factor = 0
 *            for e odd, partition(e/2) for e even
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* maximum exponent needed for n ≤ 10000 (2^13 < 10000 < 2^14) */
#define MAX_EXP 20

static seq_t part_tab[MAX_EXP + 1];
static seq_t conv_tab[MAX_EXP + 1];
static bool tables_initialized = false;

/* fast modular exponentiation for 64‑bit modulus */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1U) {
            __uint128_t t = (__uint128_t)result * base;
            result = (uint64_t)(t % mod);
        }
        __uint128_t t = (__uint128_t)base * base;
        base = (uint64_t)(t % mod);
        exp >>= 1U;
    }
    return result;
}

/* initialise partition numbers and their convolution */
static void init_tables(void)
{
    if (tables_initialized) return;

    part_tab[0] = 1;
    for (idx_t k = 1; k <= MAX_EXP; ++k) part_tab[k] = 0;

    for (idx_t i = 1; i <= MAX_EXP; ++i)
        for (idx_t j = i; j <= MAX_EXP; ++j)
            part_tab[j] += part_tab[j - i];

    for (idx_t e = 0; e <= MAX_EXP; ++e) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= e; ++k)
            sum += part_tab[k] * part_tab[e - k];
        conv_tab[e] = sum;
    }
    tables_initialized = true;
}

static seq_t local_factor(idx_t p, idx_t e)
{
    init_tables();

    if (p == 7ULL) return part_tab[(idx_t)e];
    if (p == 2ULL) return conv_tab[(idx_t)e];

    /* Legendre symbol (−7|p) */
    uint64_t base = (p - (7 % p)) % p;   /* (−7) mod p, p ≠ 7,2 */
    uint64_t leg = mod_pow64(base, (p - 1) / 2, p);
    bool splits = (leg == 1U);

    if (splits) return conv_tab[(idx_t)e];
    if ((e & 1ULL) == 1ULL) return (seq_t)0;
    return part_tab[(idx_t)(e / 2)];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 */

    init_tables();
    seq_t result = 1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t f = local_factor(p, e);
            result *= f;
            if (result == 0) return 0;
        }
    }

    if (temp > 1)        /* remaining prime factor */
        result *= local_factor(temp, 1);

    return result;
}
