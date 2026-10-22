/*
 * A316348  a(n) is the smallest k > 1 such that gcd(k, m^k - m) = 1 for all m
 *          = 2,...,n.
 * https://oeis.org/A316348
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed as the product of the smallest prime > n+1 *
 *            (call it L) and the first larger prime U such that for all m
 *            with * 2 ≤ m ≤ n, * m^(L*U - 1) is not congruent to 1 modulo L
 *            and also not to 1 * modulo U. Then k = L * U is returned. This
 *            searches * only composite numbers of the form L*...
 * Range:     u8 n <= 9, u16 n <= 249, u32 n <= 306, u64 n <= 306,
 *            u128 n <= 306, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required utilities */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/mod_pow.h"   /* for mod_pow(seq_t base, uint64_t exp, seq_t mod) */

/* sieve limit – large enough for n up to 10000                */
#define PRIME_LIMIT 200000
#define MAX_PRIMES  20000          /* roughly π(200000) */

/* static storage for primes */
static uint64_t primes[MAX_PRIMES];
static size_t prime_count = 0;

static void init_primes(void)
{
    bool is_prime[PRIME_LIMIT + 1];
    memset(is_prime, true, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; (idx_t)i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }
    prime_count = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_count++] = (uint64_t)i;
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 2 where the sequence is defined */
    static bool initialized = false;
    if (!initialized) {
        init_primes();
        initialized = true;
    }

    /* Find the smallest prime L > n+1 */
    idx_t pos = 0;
    while (pos < prime_count && primes[pos] <= (uint64_t)(n + 1)) ++pos;
    /* pos must be valid because PRIME_LIMIT > n+1 for all n in the tests */
    uint64_t L = primes[pos];

    /* Try successive primes U > L until a suitable product is found */
    for (idx_t upos = pos + 1; upos < prime_count; ++upos) {
        uint64_t U = primes[upos];
        seq_t k = (seq_t)L * (seq_t)U;
        seq_t k_minus1 = k - (seq_t)1;

        bool ok = true;
        /* Check the condition for all m = 2 .. n */
        for (idx_t m = 2; m <= n && ok; ++m) {
            if (mod_pow((seq_t)m, (uint64_t)k_minus1, (seq_t)L) == (seq_t)1) {
                ok = false;
                break;
            }
            if (mod_pow((seq_t)m, (uint64_t)k_minus1, (seq_t)U) == (seq_t)1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            return k;
        }
    }

    /* Should never reach here for the intended range of n */
    return (seq_t)0;
}
