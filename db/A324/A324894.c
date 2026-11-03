/*
 * A324894  Multiplicative with a(p^e) = p^e if sigma(p^e) is composite, and 1
 *          otherwise.
 * https://oeis.org/A324894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   A324894 – multiplicative with a(p^e) = p^e if σ(p^e) is
 *            composite, 1 otherwise
 * Range:     u8 n <= 16, u16 n <= 4225, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 107 terms through n = 9551 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    /* factor_u64 fills primes[], exps[] and sets cnt */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Multiplicative product of prime powers where σ(p^e) is composite */
    seq_t prod = 1;

    for (size_t i = 0; i < cnt; i++) {
        seq_t p = (seq_t)primes[i];
        int e = (int)exps[i];

        /* Compute p^e and (p^(e+1) - 1)/(p - 1) = σ(p^e) */
        seq_t p_pow_e = 1;
        for (int j = 0; j < e; j++) p_pow_e *= p;
        seq_t p_pow_e_plus1 = p_pow_e * p;
        seq_t sigma = (p_pow_e_plus1 - 1) / (p - 1);

        /* Include the factor if sigma is composite */
        if (!is_prime_big(sigma)) {
            prod *= p_pow_e;
        }
    }

    return prod;
}
