/*
 * A338318  Composite numbers whose prime indices are pairwise intersecting
 *          (non-coprime).
 * https://oeis.org/A338318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = composite integers whose prime indices are pairwise *
 *            intersecting (non-coprime). Computed by simple sieve and trial *
 *            factorisation with pairwise gcd checks
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* #include as needed.  All needed utilities are provided in the build. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* --- configuration ------------------------------------------------------ */
#define SIEVE_LIMIT      10000000U   /* 10 million, ample for n up to ~10k */

/* --- helpers ------------------------------------------------------------- */
static inline uint32_t gcd_uint32(uint32_t a, uint32_t b) {
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- primes and spf ----------------------------------------------------- */
static uint32_t *spf = NULL;            /* smallest prime factor, size SIEVE_LIMIT+1 */
static uint32_t  primes[700000];        /* primes up to SIEVE_LIMIT */
static size_t    primes_count = 0;      /* number of primes stored */

/* return 1-based index of prime p in the primes array (p must be prime) */
static inline uint32_t prime_pos(uint32_t p) {
    size_t lo = 0, hi = primes_count;          /* [lo,hi) */
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (uint32_t)(lo + 1);                  /* indices start at 1 */
}

/* initialise sieve and primes */
static void init_sieve(void) {
    if (spf) return;
    size_t L = SIEVE_LIMIT;
    spf = (uint32_t*)malloc((L + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    memset(spf, 0, (L + 1) * sizeof(uint32_t));

    for (uint32_t i = 2; i <= L; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if (primes_count < 700000) primes[primes_count++] = i;
            if ((uint64_t)i * i <= L) {
                for (uint64_t j = (uint64_t)i * i; j <= L; j += i) {
                    if (!spf[j]) spf[j] = i;
                }
            }
        }
    }
}

/* --- sequence cache ----------------------------------------------------- */
static seq_t *cache = NULL;   /* dynamic array of seq_t values */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* growing cache capacity when needed */
static void grow_cache(size_t required) {
    if (required <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < required) new_cap *= 2;
    seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    cache = new_arr;
    cache_cap = new_cap;
}

/* --- main function ------------------------------------------------------ */
seq_t a(idx_t n) {
    init_sieve();

    if (n == 0) return 0;          /* offset is 1; undefined for n==0 */

    static uint64_t next_candidate = 9;   /* first term in the sequence */

    while (cache_len < n) {
        /* skip even numbers */
        while ((next_candidate & 1ULL) == 0) next_candidate++;

        /* skip primes */
        if (next_candidate <= SIEVE_LIMIT && spf[next_candidate] == next_candidate) {
            next_candidate++;
            continue;
        }

        /* factorisation using spf */
        uint32_t m = (uint32_t)next_candidate;
        uint32_t indices[32];          /* indices of prime factors (with multiplicity) */
        size_t idx_len = 0;
        int bad = 0;                   /* 1 if condition fails */

        while (m > 1 && !bad) {
            uint32_t p = spf[m];
            if (p == 2) {              /* prime 2 gives index 1 → fails immediately */
                bad = 1;
                break;
            }
            uint32_t idx = prime_pos(p);
            do {
                if (idx_len < 32) indices[idx_len++] = idx;
                m /= p;
            } while (m % p == 0);
        }

        if (!bad) {
            /* pairwise gcd > 1 test */
            size_t i, j;
            for (i = 0; i < idx_len && !bad; ++i) {
                for (j = i + 1; j < idx_len; ++j) {
                    if (gcd_uint32(indices[i], indices[j]) == 1) {
                        bad = 1;
                        break;
                    }
                }
            }
        }

        if (!bad) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }

        next_candidate++;
    }

    return cache[n - 1];
}
