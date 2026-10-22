/*
 * A359908  Numbers whose prime indices have integer median.
 * https://oeis.org/A359908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: numbers whose prime indices have integer median. */
/* Sequence A359908 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- sieve data ---------- */
static uint32_t *spf = NULL;          /* smallest prime factor for each number  */
static uint32_t *prime_index = NULL;  /* index (1‑based) of each prime           */
static uint32_t *primes = NULL;       /* list of primes up to current limit     */
static size_t prime_cnt = 0;          /* number of primes stored                */
static uint64_t sieve_limit = 0;      /* current top of the sieve (inclusive)   */

/* Rebuild the sieve up to 'limit' (inclusive).  All arrays are allocated
   with appropriate casts so the code compiles in C++ as well. */
static void recompute_sieve(uint64_t limit)
{
    free(spf);
    free(prime_index);
    free(primes);

    spf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    prime_index = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    primes = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[(size_t)i] == 0) {
            spf[(size_t)i] = (uint32_t)i;
            prime_index[(size_t)i] = (uint32_t)(cnt + 1); /* 1‑based index */
            primes[cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < cnt &&
                 (uint64_t)primes[j] <= spf[(size_t)i] &&
                 i * (uint64_t)primes[j] <= limit; ++j) {
            uint64_t mult = i * (uint64_t)primes[j];
            if (spf[(size_t)mult] == 0) {
                spf[(size_t)mult] = primes[j];
            }
        }
    }

    sieve_limit = limit;
    prime_cnt   = cnt;
}

/* Ensure that the sieve has covered at least 'need'.  We grow the sieve
   by a factor of two each time it becomes insufficient. */
static void ensure_sieve(uint64_t need)
{
    if (need <= sieve_limit) return;
    uint64_t new_limit = need * 2;
    if (new_limit < 1000) new_limit = 1000;
    recompute_sieve(new_limit);
}

/* ---------- check whether an integer has integer median of prime indices ---------- */
static bool has_integer_median(uint64_t v)
{
    /* collect prime indices (with multiplicity) */
    uint32_t idxs[256];
    size_t len = 0;

    uint64_t x = v;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        if (p == 0)     /* should not happen if the sieve is large enough */
            p = (uint32_t)x;
        uint32_t idx = prime_index[(size_t)p];
        uint32_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        for (uint32_t i = 0; i < e; ++i) {
            idxs[len++] = idx;
            /* the numbers we examine are small enough that 256 entries
               are more than enough for the factor multiplicities. */
        }
    }

    if (len == 0) return false;          /* v == 1 */

    if (len & 1U) return true;           /* odd length → integer median */

    /* even length → median is (a+b)/2; integer iff a+b even.  We need
       the two middle indices.  Quick sort the small array. */
    for (size_t i = 1; i < len; ++i) {
        uint32_t key = idxs[i];
        size_t j = i;
        while (j > 0 && idxs[j - 1] > key) {
            idxs[j] = idxs[j - 1];
            --j;
        }
        idxs[j] = key;
    }

    uint32_t a = idxs[len / 2 - 1];
    uint32_t b = idxs[len / 2];
    return ((a + b) & 1U) == 0;
}

/* ---------- sequence storage ---------- */
static seq_t *terms   = NULL;    /* array of previously found terms     */
static size_t terms_len   = 0;   /* how many terms we have              */
static size_t terms_cap   = 0;   /* capacity of the terms array         */
static uint64_t last_checked = 1;/* last integer examined              */

/* Ensure the terms array can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – not part of the sequence */

    /* initialise sieve lazily */
    if (sieve_limit == 0) recompute_sieve(1000);

    /* extend the sequence until we have n terms */
    while (terms_len < (size_t)n) {
        ++last_checked;
        if (last_checked > sieve_limit) {
            ensure_sieve(last_checked * 2 + 1000);
        }
        if (has_integer_median(last_checked)) {
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)last_checked;
        }
    }
    return terms[(size_t)n - 1];
}
