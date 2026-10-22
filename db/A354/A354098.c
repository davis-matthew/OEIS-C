/*
 * A354098  Numbers k such that the cototient of k (= k-phi(k)) is a multiple
 *          of 4.
 * https://oeis.org/A354098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k (k ≥ 1) such that k−φ(k) is a multiple
 *            of 4. The offset is 1; thus a(1)=1. This implementation builds
 *            the sequence lazily by repeatedly sieving φ(k) up to the
 *            smallest power of two exceeding the current range. It is
 *            compatible with both C11 and C++17; if seq_t is a big‑i...
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if !defined(__cplusplus)
#   include <stdio.h>   /* for compilers that dislike unused functions */
#endif

static seq_t *CACHE = NULL;         /* cached sequence values */
static size_t CACHE_CAP = 0;        /* capacity of CACHE                          */
static size_t CACHE_LEN = 0;        /* number of valid entries in CACHE           */
static idx_t LAST_K = 0;            /* highest k that has been processed          */

/* Append a value to CACHE, growing its capacity if necessary.  */
static void append_cache(seq_t val)
{
    if (CACHE_LEN == CACHE_CAP) {
        size_t new_cap = CACHE_CAP ? CACHE_CAP * 2 : 16;
#ifdef __cplusplus
        seq_t *newp = new seq_t[new_cap];
        for (size_t i = 0; i < CACHE_LEN; ++i) newp[i] = CACHE[i];
        delete[] CACHE;
#else
        seq_t *newp = (seq_t*)realloc(CACHE, new_cap * sizeof(seq_t));
#endif
        CACHE = newp;
        CACHE_CAP = new_cap;
    }
    CACHE[CACHE_LEN++] = val;
}

/* Ensure that CACHE contains at least n elements.  */
static void ensure_cache(idx_t n)
{
    while (CACHE_LEN < (size_t)n) {
        idx_t limit = LAST_K ? LAST_K * 2 : 10;
        if (limit < LAST_K) limit = LAST_K + 1;   /* protect overflow */

        /* Allocate φ array.  */
        seq_t *phi;
#ifdef __cplusplus
        phi = new seq_t[limit + 1];
        for (size_t i = 0; i <= limit; ++i) phi[i] = 0;
#else
        phi = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
#endif

        /* Allocate array for primes used by linear sieve.  */
        uint64_t *primes;
        size_t prime_cap = (limit / 10) + 1;
        size_t prime_cnt = 0;
#ifdef __cplusplus
        primes = new uint64_t[prime_cap];
#else
        primes = (uint64_t*)calloc(prime_cap, sizeof(uint64_t));
#endif

        phi[1] = 1;

        /* Linear sieve for φ up to 'limit'.  */
        for (idx_t i = 2; i <= limit; ++i) {
            if (phi[i] == 0) {                     /* i is prime */
                phi[i] = (seq_t)(i - 1);
                if (prime_cnt == prime_cap) {
                    prime_cap *= 2;
#ifdef __cplusplus
                    uint64_t *tmp = new uint64_t[prime_cap];
                    for (size_t j = 0; j < prime_cnt; ++j) tmp[j] = primes[j];
                    delete[] primes;
#else
                    uint64_t *tmp = (uint64_t*)realloc(primes, prime_cap * sizeof(uint64_t));
#endif
                    primes = tmp;
                }
                primes[prime_cnt++] = i;
            }
            for (size_t j = 0; j < prime_cnt; ++j) {
                uint64_t p = primes[j];
                idx_t prod = i * p;
                if (prod > limit) break;
                if (i % p == 0) {
                    phi[prod] = phi[i] * p;
                } else {
                    phi[prod] = phi[i] * (p - 1);
                }
                if (i % p == 0) break;
            }
        }

        /* Collect qualifying k in the newly processed range.  */
        for (idx_t k = LAST_K + 1; k <= limit; ++k) {
            seq_t kseq = (seq_t)k;
            seq_t diff = kseq - phi[k];
            if ((diff & 3) == 0) {   /* cototient divisible by 4 */
                append_cache(kseq);
            }
        }

        /* Free temporary arrays.  */
#ifdef __cplusplus
        delete[] phi;
        delete[] primes;
#else
        free(phi);
        free(primes);
#endif
        LAST_K = limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; the value for n==0 is unused */
    ensure_cache(n);
    return CACHE[n - 1];
}
