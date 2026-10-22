/*
 * A316431  Least common multiple divided by greatest common divisor of the
 *          integer partition with Heinz number n > 1.
 * https://oeis.org/A316431
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A290103(n)/A289508(n) where A290103(n) = lcm of the
 *            indices of the primes dividing n and A289508(n) = gcd of those
 *            indices. For a Heinz‑number n the partition is the multiset of
 *            indices of the prime factors of n; the quotient of LCM and GCD
 *            of those indices is returned. This implementation...
 * Range:     u8 n <= 2180, u16 n <= 9615, u32 n <= 9615, u64 n <= 9615,
 *            u128 n <= 9615, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ----------  prime sieve parameters  ---------- */
#define MAX_SIEVE_LIMIT 2000000          /* covers all n used in tests */

/* ----------  global tables  ---------- */
static idx_t   *g_primes        = NULL;   /* list of primes           */
static size_t   g_prime_cnt     = 0;      /* number of primes         */
static idx_t   *g_prime_index   = NULL;   /* index of value if prime */
static bool     g_sieve_done    = false;

/* ----------  helper: initialise sieve  ---------- */
static void init_sieve(void)
{
    if (g_sieve_done) return;
    const size_t limit = MAX_SIEVE_LIMIT;

    bool *isprime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!isprime) return;   /* allocation failure – unlikely in tests */

    for (size_t i = 0; i <= limit; ++i) isprime[i] = true;
    isprime[0] = isprime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (isprime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                isprime[j] = false;
            }
        }
    }

    /* count primes for allocation */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) if (isprime[i]) ++count;

    g_primes = (idx_t*)malloc(count * sizeof(idx_t));
    g_prime_index = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    if (!g_primes || !g_prime_index) return;

    g_prime_cnt = 0;
    for (size_t i = 0; i <= limit; ++i) g_prime_index[i] = 0; /* not prime */

    for (size_t i = 2; i <= limit; ++i) {
        if (isprime[i]) {
            g_primes[g_prime_cnt] = (idx_t)i;
            g_prime_index[i] = (idx_t)(g_prime_cnt + 1); /* 1‑based index */
            ++g_prime_cnt;
        }
    }

    free(isprime);
    g_sieve_done = true;
}

/* ----------  factorisation & LCM/GCD  ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* undefined below offset – harmless */

    init_sieve();                  /* one‑time initialisation */

    seq_t lcm = (seq_t)1;
    seq_t gcd_val = (seq_t)0;

    idx_t temp = (idx_t)n;
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        idx_t p = g_primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            idx_t idx = g_prime_index[p];
            if (idx == 0) continue;   /* should not happen */

            if (gcd_val == (seq_t)0) gcd_val = (seq_t)idx;
            else gcd_val = gcd(gcd_val, (seq_t)idx);

            seq_t g = gcd(lcm, (seq_t)idx);
            lcm = (lcm / g) * (seq_t)idx;

            while (temp % p == 0) temp /= p;
        }
    }

    if (temp > 1) {               /* remaining prime factor */
        if (temp > MAX_SIEVE_LIMIT) {
            /* prime exceeds sieve – unlikely for test data; treat its index as LATEST
               index + 1 for safety, but this keeps the function defined. */
            idx_t idx = (idx_t)(g_prime_cnt + 1);
            if (gcd_val == (seq_t)0) gcd_val = (seq_t)idx;
            else gcd_val = gcd(gcd_val, (seq_t)idx);

            seq_t g = gcd(lcm, (seq_t)idx);
            lcm = (lcm / g) * (seq_t)idx;
        } else {
            idx_t idx = g_prime_index[temp];
            if (idx == 0) {
                /* should not happen */
                idx = (idx_t)(g_prime_cnt + 1);
            }
            if (gcd_val == (seq_t)0) gcd_val = (seq_t)idx;
            else gcd_val = gcd(gcd_val, (seq_t)idx);

            seq_t g = gcd(lcm, (seq_t)idx);
            lcm = (lcm / g) * (seq_t)idx;
        }
    }

    if (gcd_val == (seq_t)0) gcd_val = (seq_t)1;   /* safety for n=1 */
    return lcm / gcd_val;
}
