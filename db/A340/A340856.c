/*
 * A340856  Squarefree numbers whose greatest prime index (A061395) is
 *          divisible by their number of prime factors (A001222).
 * https://oeis.org/A340856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340856: Squarefree numbers whose greatest prime index (A061395)
   is divisible by the number of prime factors (A001222). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Upper bound for precomputation – chosen to be comfortably > the 10000‑th term. */
static const size_t CAP = 2000000;

/* Sieve data structures */
static uint32_t *spf = NULL;      /* smallest prime factor for each number */
static uint32_t *prime_idx = NULL; /* 1‑based index of a prime in the prime list */
static uint32_t *primes = NULL;   /* list of primes */
static size_t primes_cnt = 0;

/* Helper: initialise the sieve (once) */
static void init_sieve(void)
{
    if (spf) return;                /* already initialised */

    spf = (uint32_t *)malloc((CAP + 1) * sizeof(uint32_t));
    prime_idx = (uint32_t *)malloc((CAP + 1) * sizeof(uint32_t));
    primes = (uint32_t *)malloc((CAP / 2 + 1) * sizeof(uint32_t));
    if (!spf || !prime_idx || !primes) { /* allocation failure */
        exit(1);
    }

    for (uint32_t i = 2; i <= CAP; ++i) {
        if (!spf[i]) {
            spf[i] = i;                     /* i is prime */
            prime_idx[i] = ++primes_cnt;     /* 1‑based index */
            primes[primes_cnt - 1] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (p > spf[i] || prod > CAP) break;
            spf[prod] = p;
        }
    }
}

/* Return true iff x is squarefree and its greatest prime index is divisible
   by the number of distinct prime factors of x. */
static bool satisfies(uint32_t x)
{
    uint32_t temp = x;
    uint32_t omega = 0;          /* number of distinct prime factors */
    uint32_t max_idx = 0;        /* index of the largest prime factor */

    while (temp > 1) {
        uint32_t p = spf[temp];
        uint32_t exp = 0;
        /* Count exponent of p; if more than 1 → not squarefree. */
        while (temp % p == 0) {
            temp /= p;
            ++exp;
            if (exp > 1) return false;   /* not squarefree */
        }
        ++omega;
        max_idx = prime_idx[p];
    }
    /* omega > 0 because n >= 2 */
    return (max_idx % omega) == 0;
}

/* Sequence cache and construction state */
static seq_t *seq_cache = NULL;   /* growing array of sequence terms */
static size_t seq_len = 0;        /* number of terms cached */
static size_t seq_cap = 0;        /* capacity of seq_cache */
static uint32_t next_value = 2;   /* next integer to test */

/* Append a value to the sequence cache */
static void append(seq_t val)
{
    if (seq_len >= seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 64;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }
    seq_cache[seq_len++] = val;
}

/* Main entry point – returns the n‑th term (1‑based index). */
seq_t a(idx_t n)
{
    static bool init = false;
    if (!init) {
        init = true;
        init_sieve();
    }

    while (seq_len < (size_t)n) {
        if (next_value > CAP) {
            /* In practice this should never happen for n up to 10 000. */
            exit(1);
        }
        if (satisfies(next_value)) {
            append((seq_t)next_value);
        }
        ++next_value;
    }
    return seq_cache[n - 1];
}
