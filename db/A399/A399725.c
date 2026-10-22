/*
 * A399725  Numbers k that have no proper divisors d such that k - d is
 *          squarefree.
 * https://oeis.org/A399725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 7658, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A399725: numbers k such that for every proper divisor d of k, k-d is not squarefree */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/grow_seq_array.h"

/* ---------- global data ---------- */

#define PRIME_LIMIT 2000000U

static uint64_t *gPrimes   = NULL; /* primes up to PRIME_LIMIT */
static size_t   gPrimeCnt  = 0;    /* number of primes */

static seq_t *gSeq        = NULL; /* cached sequence */
static size_t gSeqCap     = 0;    /* capacity of gSeq */
static size_t gSeqLen     = 0;    /* number of values stored in gSeq */
static uint64_t gLastK    = 0;    /* last integer examined */

/* ---------- helper functions ---------- */

/* Build the list of primes up to PRIME_LIMIT using a simple sieve. */
static void init_primes(void)
{
    if (gPrimes != NULL) return;        /* already built */

    bool *isComp = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    isComp[0] = isComp[1] = true;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!isComp[i])
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                isComp[j] = true;

    /* Count primes. */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!isComp[i]) ++cnt;

    gPrimes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    gPrimeCnt = cnt;

    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!isComp[i]) gPrimes[idx++] = i;

    free(isComp);
}

/* Returns true if x is squarefree. */
static bool is_squarefree(uint64_t x)
{
    if (x < 2) return true; /* 0 and 1 are squarefree by convention */

    for (size_t i = 0; i < gPrimeCnt; ++i) {
        uint64_t p = gPrimes[i];
        uint64_t p2 = p * p;
        if (p2 > x) break;
        if (x % p2 == 0) return false;
    }
    return true;
}

/* Recursive generation of divisors of k. Returns false as soon as
   a divisor d is found for which k-d is squarefree, otherwise true. */
static bool rec_divisors(const uint64_t *pf, const int *exp, size_t pfCnt,
                         size_t idx, uint64_t cur, uint64_t k)
{
    if (idx == pfCnt) {
        if (cur == k) return true;   /* skip the number itself */
        uint64_t diff = k - cur;
        return !is_squarefree(diff); /* false if diff is squarefree */
    }

    uint64_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        if (!rec_divisors(pf, exp, pfCnt, idx + 1, val, k))
            return false;           /* bad divisor found */
        val *= pf[idx];
    }
    return true;
}

/* Checks if k is a good term of the sequence. */
static bool is_good_k(uint64_t k)
{
    if (k == 1) return true;   /* 1 has no proper divisors */

    /* Factorisation of k.  Small arrays are enough for 64‑bit numbers. */
    uint64_t pf[64];
    int      exp[64];
    size_t   cnt = 0;

    uint64_t tmp = k;
    for (size_t i = 0; i < gPrimeCnt && gPrimes[i] * gPrimes[i] <= tmp; ++i) {
        uint64_t p = gPrimes[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {  /* remaining prime factor */
        pf[cnt] = tmp;
        exp[cnt] = 1;
        ++cnt;
    }

    return rec_divisors(pf, exp, cnt, 0, 1, k);
}

/* ---------- public interface ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but harmless */

    init_primes();

    while (gSeqLen < (size_t)n) {
        ++gLastK;
        if (is_good_k(gLastK)) {
            grow_seq_array(&gSeq, &gSeqCap, gSeqLen + 1);
            gSeq[gSeqLen++] = (seq_t)gLastK;
        }
    }

    return gSeq[(size_t)n - 1];
}
