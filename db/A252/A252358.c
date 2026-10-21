/*
 * A252358  Smallest prime p such that (2^n)^(p-1) == 1 (mod p^2), i.e.,
 *          smallest Wieferich prime to base 2^n.
 * https://oeis.org/A252358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that (2^n)^(p-1) ≡ 1 (mod p^2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* Precomputed list of all primes ≤ 1093 (the known bound for the sequence) */
static uint64_t primes[200];
static size_t primes_cnt = 0;
static int primes_inited = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = 1;

    const uint64_t limit = 1093;
    uint8_t *is_comp = (uint8_t*)malloc((size_t)(limit + 1));
    if (!is_comp) return;               /* allocation failure – unlikely */

    for (uint64_t i = 0; i <= limit; ++i) is_comp[i] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }
    for (uint64_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[primes_cnt++] = i;

    free(is_comp);
}

seq_t a(idx_t n)
{
    init_primes();

    for (size_t k = 0; k < primes_cnt; ++k) {
        uint64_t p   = primes[k];
        uint64_t exp = (uint64_t)n * (p - 1);
        seq_t    mod = (seq_t)p * (seq_t)p;
        seq_t    res = mod_pow((seq_t)2, exp, mod);
        if (res == (seq_t)1) return (seq_t)p;
    }

    /* According to the definition this point is never reached for valid n, */
    /* but return 0 as a safe fallback. */
    return (seq_t)0;
}
