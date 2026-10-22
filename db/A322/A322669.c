/*
 * A322669  a(n) is the least prime(k+n-1) satisfying k > 0 and prime(k+n-1) <
 *          prime(k) + prime(k+1).
 * https://oeis.org/A322669
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime(k+n-1) with k>0 and prime(k+n-1) <
 *            prime(k)+prime(k+1) (OEIS A322669). The function performs a
 *            linear search over k and uses a prime table generated once up to
 *            a fixed bound sufficient for n up to at least 10 000
 * Range:     u8 n <= 24, u16 n <= 3030, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------
 * Prime table generation (once) – simple sieve, no external utils.
 * ------------------------------------------------------------------ */
static uint64_t *prime_table = NULL;   /* 0‑based: prime_table[0] = 2 */
static size_t   prime_count  = 0;      /* number of primes stored   */

static void init_prime_table(void)
{
    static bool done = false;
    if (done) return;

    /* Choose a bound comfortably larger than needed for n ≤ 10 000.
       370 000 is the 30 000th prime; 400 000 gives ~34 000 primes. */
    const idx_t limit = 400000;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return; /* out of memory – silently fail */

    /* Sieve of Eratosthenes */
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;
    idx_t sqrt_limit = 0;
    while ((idx_t)(sqrt_limit * sqrt_limit) <= limit) ++sqrt_limit;
    for (idx_t i = 2; i < sqrt_limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes first to know array size */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    /* Allocate prime array */
    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(is_prime);
        return;
    }

    /* Fill array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[idx++] = (uint64_t)i;

    /* Clean up */
    free(is_prime);

    prime_table = pr;
    prime_count  = cnt;
    done = true;
}

/* ------------------------------------------------------------------
 * OEIS sequence A322669.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Undefined for n < 3 – return 0 as a sentinel. */
    if (n < 3) return (seq_t)0;

    init_prime_table();
    if (!prime_table) return (seq_t)0;  /* failed to init */

    /* Linear search over k = 1, 2, … */
    for (idx_t k = 1; ; ++k) {
        /* 0-based indices into prime_table */
        idx_t idx_k   = k - 1;          /* prime(k)   */
        idx_t idx_k1  = k;              /* prime(k+1) */
        idx_t idx_kn  = k + n - 2;      /* prime(k+n-1) */

        /* Ensure we have enough primes – should hold with bound. */
        if (idx_kn >= prime_count) return (seq_t)0;

        uint64_t pk   = prime_table[idx_k];
        uint64_t pk1  = prime_table[idx_k1];
        uint64_t pkn  = prime_table[idx_kn];

        if (pkn < pk + pk1) {
            return (seq_t)pkn;  /* found the least prime satisfying the condition */
        }
    }
}
