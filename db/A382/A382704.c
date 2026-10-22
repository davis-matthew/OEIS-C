/*
 * A382704  Indices k where b(k) < k, where b is the EKG sequence A064413.
 * https://oeis.org/A382704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 699, u32 n <= 699, u64 n <= 699,
 *            u128 n <= 699, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A382704 – indices k for which the EKG sequence A064413 satisfies b(k) < k.
 * The implementation generates the EKG sequence on demand, storing the
 * qualifying indices in a growing array.  It works for any order of
 * calls and supports the generic integer types `seq_t` and `idx_t`.  All
 * memory allocation is cast appropriately for C++ compilation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Constants and global state                                         */
/* ------------------------------------------------------------------ */
static const idx_t LIMIT = 2000000;           /* should cover the required range */
static bool initialized   = false;
static idx_t *spf          = NULL;           /* smallest prime factor of i  */
static idx_t *nextCand     = NULL;           /* next unused multiple per prime*/
static uint8_t *visited    = NULL;           /* 0/1 visited flag             */
static idx_t *primeList    = NULL;           /* primes up to LIMIT           */
static size_t primeCnt     = 0;

/* Storage for the answer sequence */
static idx_t *indices      = NULL;           /* indices k with b(k) < k      */
static size_t indicesCap   = 0;
static size_t indicesSize  = 0;

/* EKG state */
static idx_t prevVal        = 0;              /* previous b(n) value          */
static idx_t currIndex      = 0;              /* next index to generate       */

/* ------------------------------------------------------------------ */
/* Helper: linear sieve for smallest prime factor and prime list     */
/* ------------------------------------------------------------------ */
static void init_sieve(void)
{
    spf        = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));
    visited    = (uint8_t*)calloc(LIMIT + 1, sizeof(uint8_t));
    nextCand   = (idx_t*)calloc(LIMIT + 1, sizeof(idx_t));
    primeList  = (idx_t*)calloc(LIMIT / 10 + 10, sizeof(idx_t));

    if (!spf || !visited || !nextCand || !primeList) return; /* allocation failed */

    spf[1] = 1;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primeList[primeCnt++] = i;
        }
        for (size_t j = 0; j < primeCnt && primeList[j] <= spf[i]
             && i * primeList[j] <= LIMIT; ++j) {
            spf[i * primeList[j]] = primeList[j];
        }
    }

    /* initialise nextCandidate for each prime */
    for (size_t i = 0; i < primeCnt; ++i)
        nextCand[primeList[i]] = primeList[i];
}

/* ------------------------------------------------------------------ */
/* Initialiser for all static data                                    */
/* ------------------------------------------------------------------ */
static void init(void)
{
    init_sieve();

    /* initialise visited with b(1)=1, b(2)=2 */
    visited[1] = visited[2] = 1;

    /* initial EKG state */
    prevVal    = 2;
    currIndex  = 3;               /* next index to generate */

    /* storage for answer indices */
    indicesCap = 16;
    indices    = (idx_t*)malloc(indicesCap * sizeof(idx_t));
    indicesSize = 0;

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Generate EKG sequence until we have at least 'need' answer terms   */
/* ------------------------------------------------------------------ */
static void ensure_up_to(size_t need)
{
    if (!initialized) init();

    static const size_t UNIQ_MAX = 16;   /* maximum distinct prime factors */
    idx_t uniq[UNIQ_MAX];
    size_t uniqCnt;

    while (indicesSize < need) {
        /* factor prevVal to obtain distinct prime factors */
        idx_t x = prevVal;
        uniqCnt = 0;
        while (x > 1) {
            idx_t p = spf[x];
            if (uniqCnt == 0 || uniq[uniqCnt - 1] != p)
                uniq[uniqCnt++] = p;
            while (x % p == 0) x /= p;
        }

        /* find minimal unused multiple among these primes */
        idx_t candidate = LIMIT + 1;
        for (size_t i = 0; i < uniqCnt; ++i) {
            idx_t p = uniq[i];
            idx_t y = nextCand[p];
            while (visited[y]) y += p;
            nextCand[p] = y;
            if (y < candidate) candidate = y;
        }

        /* mark the candidate as used */
        visited[candidate] = 1;

        /* record if b(n) < n */
        if (candidate < currIndex) {
            if (indicesSize == indicesCap) {
                indicesCap <<= 1;
                indices = (idx_t*)realloc(indices, indicesCap * sizeof(idx_t));
            }
            indices[indicesSize++] = currIndex;
        }

        /* advance state */
        prevVal    = candidate;
        ++currIndex;
    }
}

/* ------------------------------------------------------------------ */
/* Public function returning the nth term of A382704                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out-of-range */
    ensure_up_to((size_t)n);
    return (seq_t)indices[(size_t)n - 1];
}
