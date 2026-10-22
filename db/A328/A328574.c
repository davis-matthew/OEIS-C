/*
 * A328574  a(1) = 0, and, for n >= 2, numbers n whose primorial base
 *          expansion doesn't contain any nonleading zeros.
 * https://oeis.org/A328574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th non‑negative integer whose primorial‑base expansion has
   no nonleading zeros (search with memoisation).  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* list of primes sufficient for n up to about 10 000. */
static const idx_t PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL,
    59ULL, 61ULL, 67ULL, 71ULL
};
static const size_t PRIMES_CNT = sizeof(PRIMES) / sizeof(PRIMES[0]);

/* check whether m has no nonleading zeros in primorial base. */
static bool primorial_no_zero(idx_t m)
{
    if (m == 0) return true;

    unsigned __int128 prod = 1;
    size_t hi = 0;
    for (size_t i = 0; i < PRIMES_CNT; ++i) {
        idx_t p = PRIMES[i];
        if (prod > ((unsigned __int128)m) / p) break;
        prod *= p;
        hi = i + 1;
    }

    idx_t weight = (idx_t)prod;
    idx_t rem    = m;
    idx_t w      = weight;

    for (size_t pos = hi; ; --pos) {
        idx_t digit = rem / w;
        rem -= digit * w;

        if (pos < hi && digit == 0) return false;           /* non‑leading zero */
        if (digit >= PRIMES[pos]) return false;            /* digit out of range */

        if (pos == 0) break;
        w /= PRIMES[pos - 1];
    }
    return true;
}

/* cache for already  computed terms */
static seq_t  *CACHE = NULL;
static size_t CACHE_CAP = 0;
static size_t CACHE_LEN = 0;
static idx_t  NEXT_M = 1;
static bool   INITIALIZED = false;

/* extend the cache to contain at least n terms */
static void ensure_cache(idx_t n)
{
    if (!INITIALIZED) {
        CACHE_CAP = 16;
#ifdef __cplusplus
        CACHE = new seq_t[CACHE_CAP];
#else
        CACHE = (seq_t*)malloc(CACHE_CAP * sizeof(seq_t));
#endif
        if (!CACHE) exit(1);
        CACHE[0] = (seq_t)0;
        CACHE_LEN = 1;
        NEXT_M = 1;
        INITIALIZED = true;
    }

    while (CACHE_LEN < (size_t)n) {
        while (!primorial_no_zero(NEXT_M)) ++NEXT_M;
        if (CACHE_LEN >= CACHE_CAP) {
            CACHE_CAP *= 2;
#ifdef __cplusplus
            seq_t *tmp = new seq_t[CACHE_CAP];
            for (size_t i = 0; i < CACHE_LEN; ++i) tmp[i] = CACHE[i];
            delete [] CACHE;
#else
            seq_t *tmp = (seq_t*)realloc(CACHE, CACHE_CAP * sizeof(seq_t));
            if (!tmp) { free(CACHE); exit(1); }
#endif
            CACHE = tmp;
        }
        CACHE[CACHE_LEN++] = (seq_t)NEXT_M;
        ++NEXT_M;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined before offset; safe */
    ensure_cache(n);
    return CACHE[n - 1];
}
