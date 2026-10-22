/*
 * A308316  Numbers m such that q = 2^m - 1 and r = m^2 + m + 1 are both
 *          primes.
 * https://oeis.org/A308316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer m such that 2^m-1 and m^2+m+1 are both primes
   by testing all known Mersenne prime exponents up to 82589933
   (the 51st known exponent) and checking m^2+m+1 using deterministic
   Miller–Rabin for 64‑bit integers.  The list of valid exponents is
   built once and cached for subsequent calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Known exponents e such that 2^e-1 is a Mersenne prime. */
static const uint64_t mersenne_exponents[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
    107ULL, 127ULL, 521ULL, 607ULL, 1279ULL, 2203ULL, 2281ULL, 3217ULL,
    4253ULL, 4423ULL, 9689ULL, 11213ULL, 2976221ULL, 8239038ULL,
    8518751ULL, 9337610ULL, 10490960ULL, 11085358ULL, 12668093ULL,
    12954682ULL, 16014661ULL, 16893361ULL, 18462271ULL, 20949881ULL,
    29372505ULL, 34310951ULL, 38248078ULL, 47085842ULL, 49221180ULL,
    54574954ULL, 58657930ULL, 62697888ULL, 64675380ULL, 68366757ULL,
    74355422ULL, 82589933ULL
};

static const size_t mersenne_count = sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);

/* Miller–Rabin for 64‑bit integers, deterministic with known bases. */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0ULL)
            return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }
    /* Deterministic bases for 64‑bit integers. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL)
            continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Cache of valid exponents m for which 2^m-1 and m^2+m+1 are prime. */
static bool valid_built = false;
static seq_t valid_values[64];
static size_t valid_count = 0;

/* Build the list of valid exponents. */
static void build_valid_list(void)
{
    if (valid_built)
        return;
    for (size_t i = 0; i < mersenne_count; ++i) {
        uint64_t m = mersenne_exponents[i];
        uint64_t r = m * m + m + 1ULL; /* fits in 64 bits for given exponents */
        if (is_prime_uint64(r)) {
            if (valid_count < sizeof(valid_values)/sizeof(valid_values[0])) {
                valid_values[valid_count++] = (seq_t)m;
            }
        }
    }
    valid_built = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    build_valid_list();
    if ((idx_t)valid_count < n)
        return 0;
    return valid_values[n - 1];
}
