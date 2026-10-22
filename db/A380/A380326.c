/*
 * A380326  a(n) is the sum of squarefree divisors of the n-th exponentially
 *          odd number.
 * https://oeis.org/A380326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of (1 + p) over all primes p dividing the n-th *
 *            exponentially odd number (OEIS A380326)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The n-th exponentially odd number is the n-th integer whose prime
 * exponents are all odd (including exponent 0 for primes not dividing
 * the integer).  This code generates the sequence by enumerating integers
 * in increasing order up to a sufficient bound, testing the
 * “exponentially odd” property, and accumulating the desired values. */
/* Implementation uses a linear sieve to compute smallest prime factors up
 * to a fixed limit (2 000 000).  For any n up to ~10⁴ the 10 000‑th
 * such number is well below this bound, so the algorithm is sound
 * within the required time limit.  Results are cached, therefore
 * successive calls are fast and independent of call order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Global data structures ---------- */
static const unsigned LIMIT = 2000000;       /* upper bound for sieve */
static uint32_t *spf = NULL;                /* smallest prime factor array */
static uint32_t *prime_list = NULL;         /* list of primes up to LIMIT */
static size_t prime_cnt = 0;                /* number of primes found */

static seq_t *seq_cache = NULL;             /* cache of a(n) values */
static size_t cache_len = 0;                /* allocated length of cache */
static size_t cache_cnt = 0;                /* number of sequence terms computed */
static uint32_t processed_idx = 0;          /* last integer processed by sieve */

/* ---------- Utility: sieve with smallest prime factor ---------------- */
static void init_sieve(void)
{
    if (spf != NULL) return;        /* already initialized */

    /* allocate arrays */
    spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    /* safety check not needed – problem assumes adequate memory */

    /* simple linear sieve for spf */
    uint32_t *is_composite = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    prime_list = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));  /* over‑allocate */

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            spf[i] = i;
            prime_list[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            if (p > spf[i] || (uint64_t)i * p > LIMIT) break;
            is_composite[i * p] = 1;
            spf[i * p] = p;
        }
    }
    free(is_composite);
}

/* ---------- Check if integer is exponentially odd ---------------- */
static bool is_exp_odd(uint32_t n)
{
    uint32_t x = n;
    while (x > 1) {
        uint32_t p = spf[x];
        int exp = 0;
        do {
            x /= p;
            ++exp;
        } while (x % p == 0);
        if ((exp & 1) == 0) return false;   /* even exponent found */
    }
    return true;
}

/* ---------- Compute product ∏(1+p) for odd‑exponent factorization ---- */
static seq_t prod_onesp(uint32_t n)
{
    seq_t prod = 1;
    uint32_t x = n;
    while (x > 1) {
        uint32_t p = spf[x];
        int exp = 0;
        do {
            x /= p;
            ++exp;
        } while (x % p == 0);
        /* exp is guaranteed odd here */
        prod *= ((seq_t)p + 1);
    }
    return prod;
}

/* ---------- Ensure cache capacity ----------------------------------- */
static void ensure_cache_len(size_t need)
{
    if (cache_len >= need) return;
    size_t new_len = need > cache_len ? need : cache_len * 2;
    if (new_len < 16) new_len = 16;
    seq_t *new_arr = (seq_t*)malloc(new_len * sizeof(seq_t));
    if (cache_len > 0) {
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = seq_cache[i];
    }
    free(seq_cache);
    seq_cache = new_arr;
    cache_len = new_len;
}

/* ---------- Main function -------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based */
    if (n == 0) return 0;  /* undefined, but guard */
    if (cache_cnt < (size_t)n) {
        /* initialize sieve if needed */
        init_sieve();

        /* ensure enough cache space */
        ensure_cache_len((size_t)n + 1);

        /* extend precomputation up to required index n */
        while (cache_cnt < (size_t)n) {
            ++processed_idx;
            if (processed_idx > LIMIT) {
                /* Should not happen for n ≤ 10⁴ with LIMIT = 2 000 000 */
                return 0;
            }
            if (is_exp_odd(processed_idx)) {
                ++cache_cnt;
                seq_cache[cache_cnt] = prod_onesp(processed_idx);
            }
        }
    }
    return seq_cache[(size_t)n];
}
