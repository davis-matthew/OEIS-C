/*
 * A273200  Prime power-like integers.
 * https://oeis.org/A273200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime power‑like integers (OEIS A273200)
   Computes the n-th prime power‑like integer by
   checking that the difference table of its divisors
   contains only positive entries and has non‑decreasing
   rows and columns.  The function builds the sequence
   lazily and caches results.  Complexity is essentially
   O(number of tested integers). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper to sort uint64_t values */
static int cmp_uint64(const void *a, const void *b) {
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x < *y) ? -1 : (*x > *y);
}

/* Factor n; store distinct primes and exponents in the provided
   arrays.  Capacity of arrays must be at least 64. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps,
                          size_t *cnt) {
    size_t count = 0;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            do {
                n /= p;
                ++e;
            } while (n % p == 0);
            primes[count] = p;
            exps[count]   = e;
            ++count;
            if (n == 1) break;
        }
    }
    if (n > 1) {
        primes[count] = n;
        exps[count]   = 1;
        ++count;
    }
    *cnt = count;
}

/* Return true iff n>1 is prime power‑like according to the
   definition of A273200.  Uses divisor generation and
   DTD checks. */
static bool prime_power_like(uint64_t n) {
    if (n < 2) return false;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_uint64(n, primes, exps, &cnt);

    /* Generate all divisors (sorted later) */
    uint64_t divs[2048];
    size_t dsize = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t oldSize = dsize;
        for (size_t j = 0; j < oldSize; ++j) {
            uint64_t base = divs[j];
            for (uint64_t k = 1; k <= e; ++k) {
                base *= p;
                divs[dsize++] = base;
            }
        }
    }

    /* Sort divisors */
    qsort(divs, dsize, sizeof(uint64_t), cmp_uint64);

    /* Prepare for DTD checks */
    uint64_t currentRow[2048];
    size_t currentLen = dsize;
    for (size_t i = 0; i < currentLen; ++i) currentRow[i] = divs[i];

    /* Iteratively compute successive rows until length 1 */
    while (currentLen > 1) {
        size_t newLen = currentLen - 1;
        uint64_t newRow[2048];

        /* Compute differences */
        for (size_t i = 0; i < newLen; ++i) {
            uint64_t diff = currentRow[i + 1] - currentRow[i];
            if (diff == 0) return false;        /* not positive */
            newRow[i] = diff;
        }

        /* Check row nondecreasing */
        for (size_t i = 1; i < newLen; ++i)
            if (newRow[i] < newRow[i - 1]) return false;

        /* Check column nondecreasing */
        for (size_t i = 0; i < newLen; ++i)
            if (newRow[i] < currentRow[i]) return false;

        /* Prepare for next iteration */
        for (size_t i = 0; i < newLen; ++i) currentRow[i] = newRow[i];
        currentLen = newLen;
    }

    return true;
}

/* ------------------------------------------------------------------ */
/* Sequence function with lazy caching                                 */
/* ------------------------------------------------------------------ */

static seq_t *cache = NULL;          /* cached sequence values  */
static size_t cache_len = 0;         /* number of cached values */
static uint64_t next_candidate = 2;  /* next integer to test */

/* Ensure cache has at least 'needed' slots. */
static void ensure_cache_capacity(size_t needed) {
    if (cache_len >= needed) return;
    size_t new_cap = cache_len ? cache_len * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined, return 0 */

    /* Build sequence up to required index */
    while (cache_len < (size_t)n) {
        if (prime_power_like(next_candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
