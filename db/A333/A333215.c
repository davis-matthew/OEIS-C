/*
 * A333215  Lengths of maximal weakly increasing subsequences in the sequence
 *          of prime gaps (A001223).
 * https://oeis.org/A333215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by scanning the sequence of prime gaps and
 *            partitioning it into maximal weakly increasing subsequences. The
 *            function returns the length of the nth such subsequence
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state --------------------------------------------------------- */
static bool initialized = false;

static uint64_t *primes = NULL;
static size_t primeCnt  = 0;
static size_t primeCap  = 0;
static uint64_t nextPrimeCandidate = 5;   /* candidate for next prime */

static seq_t *segments = NULL;          /* stored segment lengths   */
static size_t segCnt  = 0;              /* number of completed segs */
static size_t segCap  = 0;

static idx_t     nextGapIdx       = 0;  /* next gap to process      */
static seq_t     currentSegLen    = 0;  /* length of ongoing seg    */
static uint64_t  lastGap          = 0;  /* last gap in current seg  */
static bool      segInProg        = false;

/* helper to grow the prime list -------------------------------------- */
static void grow_primes(size_t new_cap)
{
    if (primeCap >= new_cap) return;
    size_t cap = primeCap ? primeCap * 2 : 64;
    while (cap < new_cap) cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
    primes = tmp;
    primeCap = cap;
}

/* helper to grow the segment list ------------------------------------- */
static void grow_segs(size_t new_cap)
{
    if (segCap >= new_cap) return;
    size_t cap = segCap ? segCap * 2 : 64;
    while (cap < new_cap) cap *= 2;
    seq_t *tmp = (seq_t *)realloc(segments, cap * sizeof(seq_t));
    segments = tmp;
    segCap = cap;
}

/* add a new prime to the list ----------------------------------------- */
static void add_prime(uint64_t p)
{
    grow_primes(primeCnt + 1);
    primes[primeCnt++] = p;
}

/* generate next prime using trial division ------------------------------ */
static void generate_next_prime(void)
{
    uint64_t cand = nextPrimeCandidate;
    for (;; cand += 2) {
        bool ok = true;
        for (size_t i = 0; i < primeCnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { ok = false; break; }
        }
        if (ok) {
            add_prime(cand);
            nextPrimeCandidate = cand + 2;
            return;
        }
    }
}

/* lazy initialization --------------------------------------------------- */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* primes list */
    grow_primes(2);
    primes[primeCnt++] = 2;
    primes[primeCnt++] = 3;
    nextPrimeCandidate = 5;

    /* segment list */
    segCap = 0; segCnt = 0;
    segments = NULL;
}

/* main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) is first term */

    init();

    /* return cached result if already computed */
    if (segCnt >= n) return segments[n - 1];

    /* generate segments until we have at least n of them */
    while (segCnt < n) {
        /* make sure next gap exists */
        while (primeCnt <= nextGapIdx + 1)
            generate_next_prime();

        uint64_t nextGap = primes[ nextGapIdx + 1 ] - primes[ nextGapIdx ];
        ++nextGapIdx;

        if (!segInProg) {
            segInProg   = true;
            currentSegLen = 1;
            lastGap      = nextGap;
        } else {
            if (nextGap >= lastGap) {
                ++currentSegLen;
                lastGap = nextGap;
            } else {
                /* current segment finishes */
                ++segCnt;
                grow_segs(segCnt + 1);
                segments[segCnt - 1] = currentSegLen;

                /* start new segment with the current gap */
                currentSegLen = 1;
                lastGap      = nextGap;
            }
        }
    }

    return segments[n - 1];
}
