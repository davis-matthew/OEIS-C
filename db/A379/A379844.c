/*
 * A379844  Squarefree numbers x such that the product of prime indices of x
 *          is a multiple of the sum of prime indices of x.
 * https://oeis.org/A379844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A379844: squarefree numbers x such that the product of the
   prime indices dividing x is a multiple of the sum of those indices.
   The function builds the sequence once using a sieve up to a fixed
   bound large enough to contain at least 10 000 terms.  It works for
   arbitrary seq_t types (uint8_t … unsigned __int128) and idx_t is
   uint64_t.

   The algorithm:
   1. Sieve smallest prime factor (spf) and prime index arrays up to LIMIT.
   2. For each integer x ≥ 2:
        • Factor x via spf, checking for squarefreeness.
        • Collect the prime indices of the distinct prime factors.
        • Compute the sum of indices.
        • Compute product of indices modulo this sum.
        • If product % sum == 0, store x in the sequence cache.
   3. a(n) returns the (n‑1)th element of this cache (1‑based).
*/

#include <stdint.h>
#include <stdlib.h>

/* ---------- Global cache of the sequence -------------------------------- */
static seq_t *seq_cache = NULL;        /* array of sequence values          */
static size_t seq_len   = 0;           /* number of stored terms            */
static size_t seq_cap   = 0;           /* allocated capacity of seq_cache   */

/* ---------- Helper to allocate or grow an array ------------------------- */
static void grow_seq_cache(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < needed)
            new_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ---------- Build the sequence up to a fixed upper bound ----------------- */
static void build_sequence(void)
{
    /* A bound that comfortably contains >10 000 terms. */
    const uint32_t LIMIT = 5000000U;   /* 5 million */

    /* Arrays for sieve */
    uint32_t *spf = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    uint32_t *prime_index = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    uint32_t *primes = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));  /* ample size */
    size_t primes_cnt = 0;

    /* Build spf and prime_index using linear sieve */
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
            prime_index[i] = (uint32_t)primes_cnt;  /* 1‑based index */
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (p > spf[i] || ip > LIMIT)
                break;
            spf[(uint32_t)ip] = p;
        }
    }

    /* Temporary buffer for indices of prime factors of a number. */
    /* 10 is enough for numbers ≤ 5 million (max 7 distinct primes). */
    uint32_t idx_arr[10];
    const uint32_t MAX_PRIMES = 10;

    /* Main loop: test each integer up to LIMIT. */
    for (uint32_t x = 2; x <= LIMIT; ++x) {
        uint32_t n = x;
        uint32_t len = 0;
        int squarefree = 1;

        /* Factor n using spf, collecting prime indices. */
        while (n > 1) {
            uint32_t p = spf[n];
            uint32_t cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
            }
            if (cnt > 1) {          /* repeated factor → not squarefree */
                squarefree = 0;
                break;
            }
            if (len < MAX_PRIMES) {
                idx_arr[len++] = prime_index[p];
            } else {                /* should not happen for LIMIT ≤ 5 M */
                squarefree = 0;
                break;
            }
        }
        if (!squarefree)
            continue;

        /* Sum of prime indices. */
        uint32_t sum = 0;
        for (uint32_t i = 0; i < len; ++i)
            sum += idx_arr[i];
        if (sum == 0)
            continue;   /* should not happen */

        /* Compute product modulo sum. */
        uint64_t prod_mod = 1 % sum;
        for (uint32_t i = 0; i < len; ++i)
            prod_mod = (prod_mod * idx_arr[i]) % sum;

        if (prod_mod == 0) {
            grow_seq_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)x;
        }
    }

    /* Free temporary arrays. */
    free(spf);
    free(prime_index);
    free(primes);
}

/* ---------- Sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialization on first call. */
    if (seq_cache == NULL)
        build_sequence();

    /* 1-based indexing; return 0 for out‑of‑range n==0. */
    if (n == 0)
        return 0;
    return seq_cache[(size_t)(n - 1)];
}
