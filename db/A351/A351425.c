/*
 * A351425  If n = p_1^e_1 * ... * p_k^e_k, where p_1 < ... < p_k are primes,
 *          then a(n) is obtained by replacing the last factor p_k^e_k by (p_k
 *          + 1)^(e_k - 1); a(1) = 1.
 * https://oeis.org/A351425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n / (p_k^e_k)) * (p_k + 1)^(e_k - 1) where n = p_1^e_1 *
 *            ... * p_k^e_k (p_1 < ... < p_k primes). a(1) = 1. Computes the
 *            OEIS sequence A351425
 * Range:     u8 n <= 127, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"  /* provides build_primes_upto() */
#include "../utils/seq_pow.h"  /* provides seq_pow(seq_t, idx_t) */

static bool primes_initialized = false;
static uint64_t *primes = NULL;
static size_t prime_count = 0;

/* Build a prime list up to LIMIT once, for subsequent factorisations. */
static void init_primes(void) {
    if (primes_initialized) return;
    const idx_t LIMIT = 100000; /* enough for n up to ~10^10; far more than needed (cur test range 10k) */
    build_primes_upto(LIMIT, &primes, &prime_count);
    primes_initialized = true;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;                 /* a(1) = 1 */

    init_primes();                               /* ensure prime list ready */

    idx_t orig = n;                              /* keep original value */
    idx_t temp = n;                              /* for factorisation */

    /* Track largest prime factor and its exponent. */
    uint64_t last_p = 0;
    idx_t      last_e = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;

        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            last_p = p;
            last_e = e;                         /* update with current prime (will be last) */
        }
    }

    if (temp > 1) {                              /* leftover large prime factor */
        last_p = temp;
        last_e = 1;
    }

    /* Compute m = orig / (last_p^last_e) */
    seq_t m = (seq_t)orig;
    seq_t last_power = seq_pow((seq_t)last_p, last_e);
    m = m / last_power;

    /* Compute (last_p + 1)^(last_e - 1) */
    seq_t term = seq_pow((seq_t)(last_p + 1), last_e > 0 ? last_e - 1 : 0);

    return m * term;
}
