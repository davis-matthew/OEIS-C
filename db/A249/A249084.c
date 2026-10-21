/*
 * A249084  Number of length n+5 0..7 arrays with every six consecutive terms
 *          having two times the sum of some two elements equal to the sum of
 *          the remaining four.
 * https://oeis.org/A249084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of length n+5 arrays over {0..7} where for every *
 *            consecutive 6 terms there exists a pair whose sum equals twice
 *            the sum * of some other two (i.e., the sum of the remaining
 *            four). This * function returns that number for any n ≥ 1. The
 *            implementation uses * linear‑time dynamic programming...
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 35, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
#  include <new>
#endif

/* Number of possible 5‑tuples of digits 0..7 (3 bits each).  */
#define STATES (1 << 15)   /* 8^5 = 32768 */

/* ------------------------------------------------------------------ */
/* Helper allocation functions that work in C and C++                */
/* ------------------------------------------------------------------ */
static inline seq_t *allocate_seq_array(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n]();          /* zero‑initialized */
#else
    return (seq_t*)calloc(n, sizeof(seq_t));
#endif
}

static inline void free_seq_array(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

static inline seq_t *realloc_seq_array(seq_t *oldp, size_t oldsize, size_t newsize)
{
#ifdef __cplusplus
    seq_t *newp = new seq_t[newsize]();  /* zero‑init new elements */
    for (size_t i = 0; i < oldsize; ++i) newp[i] = oldp[i];
    delete[] oldp;
    return newp;
#else
    seq_t *newp = (seq_t*)realloc(oldp, newsize * sizeof(seq_t));
    if (!newp) abort();
    if (newsize > oldsize)
        memset(newp + oldsize, 0, (newsize - oldsize) * sizeof(seq_t));
    return newp;
#endif
}

/* ------------------------------------------------------------------ */
/* Transition tables: for each 5‑tuple state, the allowed next
 *  values (bitmask) and the resulting state after sliding the window.   */
/* ------------------------------------------------------------------ */
static uint8_t  goodMask[STATES];
static int16_t nextState[STATES][8];

/* ------------------------------------------------------------------ */
/* DP arrays: counts of arrays ending with each 5‑tuple state.        */
/* ------------------------------------------------------------------ */
static seq_t *currCnt = NULL;
static seq_t *nextCnt = NULL;

/* ------------------------------------------------------------------ */
/* Cached sequence values a(n).  Index n is stored at seqCache[n].    */
/* ------------------------------------------------------------------ */
static seq_t *seqCache = NULL;
static size_t cacheSize = 0;      /* allocated size of seqCache     */
static idx_t   currentLen = 0;    /* length of arrays processed so far */

/* ------------------------------------------------------------------ */
/* Forward declaration of one‑time initialisation.                    */
/* ------------------------------------------------------------------ */
static void init_seq_a249084(void);

/* ------------------------------------------------------------------ */
/* Main entry point.                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for undefined case */

    /* initialise on first call */
    if (seqCache == NULL) init_seq_a249084();

    /* desired length of arrays */
    idx_t targetLen = n + 5;
    if (targetLen > currentLen)
    {
        /* enlarge cache if necessary */
        if ((size_t)(n + 1) > cacheSize)
        {
            size_t newSize = cacheSize ? cacheSize << 1 : 128;
            while ((size_t)(n + 1) > newSize) newSize <<= 1;
            seqCache = realloc_seq_array(seqCache, cacheSize, newSize);
            cacheSize = newSize;
        }

        /* Expand DP from currentLen up to targetLen */
        while (currentLen < targetLen)
        {
            idx_t nextLen = currentLen + 1;          /* length after adding one element */

            /* zero nextCnt */
            memset(nextCnt, 0, STATES * sizeof(seq_t));

            /* transition */
            for (int s = 0; s < STATES; ++s)
            {
                seq_t cnt = currCnt[s];
                if (!cnt) continue;
                uint8_t mask = goodMask[s];
                if (!mask) continue;
                unsigned int m = mask;
                while (m)
                {
                    int v = __builtin_ctz(m);
                    m &= m - 1;
                    int16_t ns = nextState[s][v];
                    if (ns >= 0) nextCnt[ns] += cnt;
                }
            }

            /* compute total for new length */
            seq_t total = 0;
            for (int s = 0; s < STATES; ++s) total += nextCnt[s];

            /* cache result: a(nextLen) = total */
            seqCache[(size_t)nextLen - 5] = total;

            /* prepare for next iteration */
            seq_t *tmp = currCnt;
            currCnt = nextCnt;
            nextCnt = tmp;
            currentLen = nextLen;
        }
    }

    return seqCache[(size_t)n];
}

/* ------------------------------------------------------------------ */
/* One‑time initialisation: build transition tables and initial DP    */
/* state corresponding to arrays of length 6 (n = 1).                  */
/* ------------------------------------------------------------------ */
static void init_seq_a249084(void)
{
    /* allocate status arrays */
    currCnt = allocate_seq_array(STATES);
    nextCnt = allocate_seq_array(STATES);

    /* Precompute transitions for all 5‑tuple states. */
    for (int s = 0; s < STATES; ++s)
    {
        uint8_t a = (s >> 12) & 7;
        uint8_t b = (s >> 9)  & 7;
        uint8_t c = (s >> 6)  & 7;
        uint8_t d = (s >> 3)  & 7;
        uint8_t e =  s        & 7;

        goodMask[s] = 0;

        for (int v = 0; v < 8; ++v)
        {
            int sum6 = a + b + c + d + e + v;
            if (sum6 % 3 != 0) { nextState[s][v] = -1; continue; }
            int target = sum6 / 3;

            bool ok = false;
            int vals6[6] = {a, b, c, d, e, v};
            for (int i = 0; i < 6 && !ok; ++i)
                for (int j = i + 1; j < 6; ++j)
                    if (vals6[i] + vals6[j] == target) { ok = true; break; }

            if (ok)
            {
                goodMask[s] |= (1U << v);
                int ns = ((b << 12) | (c << 9) | (d << 6) | (e << 3) | v);
                nextState[s][v] = (int16_t)ns;
            }
            else
                nextState[s][v] = -1;
        }
    }

    /* Initial DP for arrays of length 6 (n = 1). */
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 8; ++b)
            for (int c = 0; c < 8; ++c)
                for (int d = 0; d < 8; ++d)
                    for (int e = 0; e < 8; ++e)
                        for (int f = 0; f < 8; ++f)
                        {
                            int sum6 = a + b + c + d + e + f;
                            if (sum6 % 3 != 0) continue;
                            int target = sum6 / 3;
                            bool ok = false;
                            int vals6[6] = {a, b, c, d, e, f};
                            for (int i = 0; i < 6 && !ok; ++i)
                                for (int j = i + 1; j < 6; ++j)
                                    if (vals6[i] + vals6[j] == target) { ok = true; break; }
                            if (!ok) continue;
                            int state = ((b << 12) | (c << 9) | (d << 6) | (e << 3) | f);
                            currCnt[state] += 1;
                        }

    /* Compute total for n = 1 and cache it. */
    seq_t total0 = 0;
    for (int s = 0; s < STATES; ++s) total0 += currCnt[s];

    /* Initialise cache. */
    cacheSize = 128;
    seqCache = allocate_seq_array(cacheSize);
    seqCache[1] = total0;

    /* Set initial DP state for next length. */
    currentLen = 6;
}
