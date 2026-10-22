/*
 * A332832  Heinz numbers of integer partitions whose negated first
 *          differences (assuming the last part is zero) are not unimodal.
 * https://oeis.org/A332832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest integer whose prime‐factor indices form
 *            a partition whose negated first differences (with the last part
 *            assumed zero) are NOT unimodal. The sequence is obtained by
 *            scanning the natural numbers and selecting those that satisfy
 *            this property. The returned values are Heinz nu...
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Required types are supplied by the build:  seq_t, idx_t  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Size of the sieve used for factorisation. */
#define MAX_SIEVE 10000000UL   /* 10 million is ample for first 10 000 terms */

/* Number of terms precomputed.  It is larger than 10 000 to avoid
   expensive extensions. */
#define TARGET_TERMS 15000UL

/* ------------------------------------------------------------------ */
/*  Global data for the sieve and prime list.  (static storage)       */
/* ------------------------------------------------------------------ */
static uint32_t *spf = NULL;        /* smallest prime factor for every n up to MAX_SIEVE */
static uint32_t *primes = NULL;     /* list of primes up to MAX_SIEVE (1‑based indices) */
static size_t prime_cnt = 0;        /* number of primes found */

/* Cache of the sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool cache_init = false;

/* ------------------------------------------------------------------ */
/*  Helper: Build the sieve of Eratosthenes to obtain spf and primes.  */
/*  Complexity:  O(MAX_SIEVE).                                          */
/* ------------------------------------------------------------------ */
static void build_sieve(void)
{
    uint32_t limit = (uint32_t)MAX_SIEVE;
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) abort();
    for (uint32_t i = 0; i <= limit; ++i) spf[i] = 0;

    /* Allocate room for the prime array.  Rough assumption: at most limit/10 primes. */
    primes = (uint32_t *)malloc((limit / 10 + 1) * sizeof(uint32_t));
    if (!primes) abort();
    size_t pi = 0;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            primes[pi++] = i;
        }
        for (size_t j = 0; j < pi; ++j) {
            uint32_t p = primes[j];
            uint64_t m = (uint64_t)i * p;
            if (m > limit) break;
            spf[(uint32_t)m] = p;
            if (p == spf[i]) break;
        }
    }
    prime_cnt = pi;

    /* Shrink the array to the exact size. */
    uint32_t *tmp = (uint32_t *)realloc(primes, prime_cnt * sizeof(uint32_t));
    if (tmp) primes = tmp;
}

/* ------------------------------------------------------------------ */
/*  Helper: Return the 1‑based index of prime p in the pre‑computed
   prime list.  Returns -1 if p is not prime (should never happen).    */
/* ------------------------------------------------------------------ */
static int prime_index(uint32_t p)
{
    size_t low = 0, high = prime_cnt;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        uint32_t v = primes[mid];
        if (v < p) low = mid + 1;
        else high = mid;
    }
    if (low < prime_cnt && primes[low] == p) return (int)(low + 1);   /* 1‑based */
    return -1;
}

/* ------------------------------------------------------------------ */
/*  Helper: Determine whether a sequence of non‑negative integers
   (a[0..len-1]) is unimodal (weakly increasing then weakly decreasing).  */
/* ------------------------------------------------------------------ */
static bool is_unimodal(const int *a, size_t len)
{
    if (len <= 2) return true;   /* any sequence of length ≤2 is unimodal */

    bool inc_phase = true;
    for (size_t i = 1; i < len; ++i) {
        if (inc_phase) {
            if (a[i] < a[i-1]) inc_phase = false;   /* switch to decreasing phase */
        } else {
            if (a[i] > a[i-1]) return false;        /* increase after decreasing → not unimodal */
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Test whether the integer n belongs to the sequence.  The integer
   is first factorised; the multiplicities of the prime indices form
   the partition.  The partition is acceptable iff its negated first
   differences are NOT unimodal.  The function returns true when
   n belongs to the sequence.                */
/* ------------------------------------------------------------------ */
static bool passes(uint32_t n)
{
    /* Extract the multiset of prime indices from the factorisation of n. */
    int parts[64];          /* sufficient for expected lengths (see analysis) */
    size_t parts_len = 0;
    uint32_t v = n;

    while (v > 1) {
        uint32_t p = spf[v];
        int exp = 0;
        while (v % p == 0) {
            v /= p;
            ++exp;
        }
        int idx = prime_index(p);
        for (int k = 0; k < exp; ++k) {
            parts[parts_len++] = idx;
        }
    }

    if (parts_len <= 2) return false;   /* partitions with ≤2 parts are always unimodal */

    /* Sort the parts in non‑increasing order. */
    /* Simple insertion sort for small array sizes. */
    for (size_t i = 1; i < parts_len; ++i) {
        int key = parts[i];
        size_t j = i;
        while (j > 0 && parts[j-1] < key) {
            parts[j] = parts[j-1];
            --j;
        }
        parts[j] = key;
    }

    /* Compute the negated first differences (parts[i] - parts[i+1], with parts[parts_len] = 0). */
    int diffs[64];
    for (size_t i = 0; i < parts_len; ++i) {
        int next = (i + 1 < parts_len) ? parts[i+1] : 0;
        diffs[i] = parts[i] - next;
    }

    /* Sequence is acceptable iff it is NOT unimodal. */
    return !is_unimodal(diffs, parts_len);
}

/* ------------------------------------------------------------------ */
/*  Pre‑compute the first TARGET_TERMS values of the sequence and store
   them in the static cache.  The routine is idempotent.                 */
/* ------------------------------------------------------------------ */
static void ensure_cache(void)
{
    if (cache_init) return;
    build_sieve();   /* initialise spf and primes once */

    cache = (seq_t *)malloc(TARGET_TERMS * sizeof(seq_t));
    if (!cache) abort();
    cache_len = 0;

    for (uint32_t i = 2; cache_len < TARGET_TERMS; ++i) {
        if (passes(i)) {
            cache[cache_len++] = (seq_t)i;
        }
    }
    cache_init = true;
}

/* ------------------------------------------------------------------ */
/*  The exported function required by the build system.                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; behaviour for offset<1 is irrelevant */

    ensure_cache();

    /* If the caller wants a term beyond the pre‑computed range, extend the search. */
    if ((size_t)n > cache_len) {
        uint32_t current = cache_len ? (uint32_t)cache[cache_len - 1] + 1 : 2;
        while ((size_t)n > cache_len) {
            if (passes(current)) {
                seq_t *tmp = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
                if (!tmp && (cache_len + 1) * sizeof(seq_t) != 0) abort();
                cache = tmp;
                cache[cache_len++] = (seq_t)current;
            }
            ++current;
        }
    }

    return cache[(size_t)n - 1];
}
