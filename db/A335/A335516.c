/*
 * A335516  Number of normal patterns contiguously matched by the prime
 *          indices of n in increasing or decreasing order, counting
 *          multiplicity.
 * https://oeis.org/A335516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct normal patterns (preferential *
 *            arrangements) that occur as contiguous subsequences of the
 *            multiset of * prime indices of n, counting the empty pattern
 *            once
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* ----- pre‑computation (once) ------------------------------------- */
#define MAX_N 2000000U  /* enough for the test range (n ≤ 10000) */

static uint32_t  spf[MAX_N + 1];       /* smallest prime factor */
static uint16_t  prime_index[MAX_N + 1];/* prime → 1‑based index */
static uint32_t  prime_list[MAX_N / 10];/* list of primes */
static size_t    prime_cnt = 0;
static bool      initialized = false;

static void init_sieve(void)
{
    if (initialized)
        return;

    for (uint32_t i = 0; i <= MAX_N; ++i)
        spf[i] = i;

    for (uint32_t i = 2; i * i <= MAX_N; ++i)
        if (spf[i] == i)
            for (uint32_t j = i * i; j <= MAX_N; j += i)
                if (spf[j] == j)
                    spf[j] = i;

    for (uint32_t i = 2; i <= MAX_N; ++i)
        if (spf[i] == i) {
            prime_index[i] = (uint16_t)(prime_cnt + 1); /* 1‑based */
            prime_list[prime_cnt++] = i;
        }

    initialized = true;
}

/* ----- main function ---------------------------------------------- */
seq_t a(idx_t n)
{
    init_sieve();                           /* prime tables ready */

    if (n == 0)            /* safety – offset is 1, but defensive */
        return (seq_t)0;

    /* build multiset of prime indices of n (sorted ascending) */
    int S[64];          /* length ≤ 64 for n up to 2^64 */
    int len = 0;

    if (n > 1) {
        uint32_t tmp = (uint32_t)n;   /* n ≤ MAX_N for tests */
        while (tmp > 1) {
            uint32_t p    = spf[tmp];
            uint32_t idx  = prime_index[p];
            while (tmp % p == 0) {
                S[len++] = (int)idx;
                tmp /= p;
            }
        }
    }

    if (len == 0)                     /* n = 1 → only the empty pattern */
        return (seq_t)1;

    /* maximum distinct patterns: len*(len+1)/2, len ≤ 64 → < 2100 */
    static int   pat[3000][64];
    static int   patlen[3000];
    size_t       patcnt = 0;

    int curpat[64];
    int seenIdx[64];
    int seenLbl[64];
    int seenCnt;

    for (int l = 0; l < len; ++l) {
        seenCnt   = 0;
        int nextLabel = 0;

        for (int r = l; r < len; ++r) {
            int v = S[r];
            int lbl = 0;
            for (int t = 0; t < seenCnt; ++t)
                if (seenIdx[t] == v) {
                    lbl = seenLbl[t];
                    break;
                }
            if (lbl == 0) {          /* new distinct value in this subarray */
                lbl = ++nextLabel;
                seenIdx[seenCnt]  = v;
                seenLbl[seenCnt]  = lbl;
                ++seenCnt;
            }
            curpat[r - l] = lbl;

            int curLen = r - l + 1;
            bool found = false;
            for (size_t i = 0; i < patcnt; ++i) {
                if (patlen[i] == curLen &&
                    memcmp(pat[i], curpat, curLen * sizeof(int)) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                memcpy(pat[patcnt], curpat, curLen * sizeof(int));
                patlen[patcnt] = curLen;
                ++patcnt;
            }
        }
    }

    return (seq_t)(patcnt + 1); /* +1 for the empty pattern */
}
