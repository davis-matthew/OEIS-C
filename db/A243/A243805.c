/*
 * A243805  Integers k such that 6 is a 6th power mod k.
 * https://oeis.org/A243805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 7353, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243805 – integers k such that 6 is a 6th power mod k.
 *  The term a(n) is the nth integer (1‑based) that satisfies:
 *    • k is not divisible by 4 or 9,
 *    • for every odd prime divisor p of k, 6 is a 6th power residue mod p
 *      (i.e. 6^((p-1)/gcd(p-1,6)) ≡ 1 (mod p)).
 *
 *  The function performs a linear search over the positive integers,
 *  testing each candidate against these conditions.  A small prime sieve
 *  up to 1‑000‑000 is built once and reused for factorisation.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Utility for modular exponentiation (base^exp mod mod) using 128‑bit
 * arithmetic to avoid overflow on uint64_t operands.
 */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp != 0) {
        if (exp & 1ULL)
            result = (uint64_t)(((__uint128_t)result * base) % mod);
        base = (uint64_t)(((__uint128_t)base * base) % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Euclidean algorithm for gcd of two uint64_t values. */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Test whether an odd prime p (>3) allows 6 to be a 6th power residue
 * modulo p.  2 and 3 are always allowed.
 */
static bool is_allowed_prime(uint64_t p)
{
    if (p <= 3) return true;             /* 2 and 3 pass automatically */
    uint64_t d   = gcd64(p - 1, 6ULL);
    uint64_t exp = (p - 1) / d;
    return mod_pow64(6ULL, exp, p) == 1;
}

/* ------------------------------------------------------------------ */
/* Small prime sieve – primes up to 1 000 000 are precomputed
 * once on first use.  The primes are stored in a static array.
 */
#define PRIME_LIMIT   1000000
#define MAX_PRIMES    80000   /* < π(1 000 000) ≈ 78 498 */
static int primes_arr[MAX_PRIMES];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    /* Allocate sieve array – zero means prime, one means composite. */
    static unsigned char is_comp[PRIME_LIMIT + 1];
    for (int i = 2; i <= PRIME_LIMIT; ++i) is_comp[i] = 0;

    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    for (int i = 2; i <= PRIME_LIMIT && primes_cnt < MAX_PRIMES; ++i) {
        if (!is_comp[i]) primes_arr[primes_cnt++] = i;
    }
}

/* ------------------------------------------------------------------ */
/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 – caller must use n >= 1 */

    init_primes();

    idx_t   found = 0;          /* number of admissible integers found */
    uint64_t cand  = 1;         /* current candidate integer */

    for (;;) {
        /* Reject numbers divisible by 4 or 9. */
        if ((cand & 3ULL) == 0 || (cand % 9ULL) == 0) {
            ++cand;
            continue;
        }

        uint64_t m = cand;

        /* Handle factor 2 (allow at most one factor of 2). */
        if ((m & 1ULL) == 0) {
            m >>= 1;               /* divide by 2 once */
            if ((m & 1ULL) == 0) { /* still even => exponent >= 2 => reject */
                ++cand;
                continue;
            }
        }

        /* Handle factor 3 (allow at most one factor of 3). */
        if (m % 3ULL == 0) {
            m /= 3ULL;             /* divide by 3 once */
            if (m % 3ULL == 0) {    /* still divisible by 3 => exponent >= 2 */
                ++cand;
                continue;
            }
        }

        /* Test remaining odd prime factors. */
        bool ok = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            int  p    = primes_arr[i];
            uint64_t up = (uint64_t)p;
            if (up * up > m) break;
            if (m % up == 0) {
                if (!is_allowed_prime(up)) { ok = false; break; }
                while (m % up == 0) m /= up;  /* skip all copies of p */
            }
        }
        if (!ok) { ++cand; continue; }

        /* If a prime remains after factoring up to sqrt(original m). */
        if (m > 1 && !is_allowed_prime(m)) { ++cand; continue; }

        /* Candidate satisfies all conditions. */
        ++found;
        if (found == n) return (seq_t)cand;
        ++cand;
    }
}
