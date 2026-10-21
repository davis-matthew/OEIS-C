/*
 * A254750  Numbers such that, in base 10, all their proper prefixes and
 *          suffixes represent composites.
 * https://oeis.org/A254750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 3774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A254750 – numbers whose every proper prefix and suffix is composite.
   Computed by brute‑force search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/digits_of_seq.h"

/* --- Helpers ----------------------------------------------------- */
static const size_t LIMIT = 4000000;            /* sieve limit */

/* isPrime[i] == true  -> i is prime  (i >= 2)
 * isPrime[i] == false -> i is composite */
static bool *isPrime = NULL;

/* cache of the sequence */
static seq_t *cache = NULL;
static size_t cacheLen = 0;   /* number of terms stored */
static size_t cacheCap = 0;

/* sieve of Eratosthenes up to LIMIT (build once) */
static void init_sieve(void)
{
    if (isPrime != NULL)
        return;  /* already initialized */

    isPrime = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (isPrime == NULL)
        exit(1);  /* memory allocation failed */

    /* Assume all numbers >=2 are prime at start */
    for (size_t i = 2; i <= LIMIT; ++i)
        isPrime[i] = true;

    /* integer sqrt of LIMIT */
    size_t sqrtLimit = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        sqrtLimit = i;

    /* Sieve */
    for (size_t i = 2; i <= sqrtLimit; ++i) {
        if (isPrime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                isPrime[j] = false;
        }
    }
}

/* test whether x is composite (x>1) */
static bool is_composite(seq_t x)
{
    if (x <= (seq_t)1)
        return false;
    if (x <= (seq_t)LIMIT)
        return !isPrime[(size_t)x];
    return !is_prime_big(x);
}

/* test if a number satisfies the sequence property */
static bool valid_candidate(seq_t x)
{
    uint8_t digs[64];
    size_t len;
    digits_of_seq(x, digs, &len);

    if (len < 2)   /* proper prefixes and suffixes must exist */
        return false;

    /* prefixes */
    seq_t pref = 0;
    for (size_t i = 0; i < len - 1; ++i) {
        pref = pref * 10 + digs[i];
        if (!is_composite(pref))
            return false;
    }

    /* suffixes */
    for (size_t split = 1; split < len; ++split) {
        seq_t suff = 0;
        for (size_t j = split; j < len; ++j)
            suff = suff * 10 + digs[j];
        if (!is_composite(suff))
            return false;
    }
    return true;
}

/* Sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;     /* out-of-range, return 0 */

    /* initialise on first call */
    init_sieve();

    /* lazy initialization of cache start value */
    static seq_t nextCandidate = (seq_t)44;

    /* grow cache if needed */
    while (cacheLen < (size_t)n) {
        /* find next valid candidate */
        while (!valid_candidate(nextCandidate))
            ++nextCandidate;

        /* store it */
        if (cacheLen == cacheCap) {
            cacheCap = (cacheCap == 0) ? 128 : cacheCap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, cacheCap * sizeof(seq_t));
            if (tmp == NULL)
                exit(1);      /* memory allocation failed */
            cache = tmp;
        }
        cache[cacheLen++] = nextCandidate;
        ++nextCandidate;
    }

    return cache[n - 1];
}
