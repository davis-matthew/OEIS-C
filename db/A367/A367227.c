/*
 * A367227  Numbers m whose prime indices have no nonnegative linear
 *          combination equal to bigomega(m).
 * https://oeis.org/A367227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers m whose prime indices have no nonnegative linear combination
   equal to bigomega(m).  Implemented by searching odd integers and testing
   representability. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* ------------------------------------------------------------------ */
/* --- Static state -------------------------------------------------- */
/* ------------------------------------------------------------------ */

/* Cache for sequence values */
static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;          /* number of terms currently stored */
static size_t seq_cap     = 0;          /* allocated capacity */

/* Table of primes (used for factorization and prime‑indices) */
static uint64_t *prime_list = NULL;
static size_t prime_count    = 0;
static bool primes_ready     = false;

/* ------------------------------------------------------------------ */
/* --- Helper functions ---------------------------------------------- */
/* ------------------------------------------------------------------ */

/* Initialise the prime list once, up to a generous bound. */
static void init_primes(void)
{
    if (primes_ready) return;
    size_t cnt;
    uint64_t *pr;
    /* 1 000 000 is more than enough for n ≤ 10 000 */
    build_primes_upto(1000000, &pr, &cnt);
    prime_list  = pr;
    prime_count = cnt;
    primes_ready = true;
}

/* Binary search for the 1‑based index of prime p in prime_list. */
static int prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (int)(mid + 1); /* indices start at 1 */
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return -1; /* not found – should not happen for primes ≤ 1 000 000 */
}

/* Extend the cache until it contains at least min_len terms. */
static void extend_cache(size_t min_len)
{
    static uint64_t cand = 3;   /* candidate odd integer to test */

    while (seq_len < min_len) {
        /* Factorise cand.  We know it fits in uint64_t (it will be far below). */
        uint64_t pf[64], ex[64];
        size_t fc = 0;
        factor_u64(cand, pf, ex, &fc);

        /* Compute bigomega(cand) = sum of exponents. */
        int bigomega = 0;
        for (size_t i = 0; i < fc; ++i) bigomega += (int)ex[i];

        /* Collect distinct prime indices (duplicates discarded). */
        int idxs[64];
        size_t idx_cnt = 0;
        for (size_t i = 0; i < fc; ++i) {
            int idx = prime_index(pf[i]);
            if (idx < 1) continue;           /* safety guard */
            bool seen = false;
            for (size_t j = 0; j < idx_cnt; ++j)
                if (idxs[j] == idx) { seen = true; break; }
            if (!seen) idxs[idx_cnt++] = idx;
        }

        /* Unbounded coin change DP to test representability. */
        bool representable = false;
        if (bigomega > 0) {
            /* reach[s] == 1 indicates that sum s can be represented */
            char *reach = (char *)malloc((size_t)bigomega + 1);
            reach[0] = 1;
            for (int s = 1; s <= bigomega; ++s) reach[s] = 0;
            for (size_t i = 0; i < idx_cnt; ++i) {
                int val = idxs[i];
                for (int s = val; s <= bigomega; ++s)
                    if (reach[s - val]) reach[s] = 1;
            }
            representable = reach[bigomega] != 0;
            free(reach);
        }

        /* If not representable, store the candidate in the cache. */
        if (!representable) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)cand;
        }

        cand += 2;   /* next odd integer */
    }
}

/* ------------------------------------------------------------------ */
/* --- Public function ----------------------------------------------- */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for index 0 */

    init_primes();

    if (seq_cache == NULL) {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    /* Ensure cache holds at least n terms */
    if (seq_len < (size_t)n)
        extend_cache((size_t)n);

    return seq_cache[n - 1];
}
