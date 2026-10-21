/*
 * A264970  If A262686(n) = 0, a(n) = 0, otherwise a(n) = 1 + a(A262686(n)),
 *          where A262686(n) = largest number k such that k - d(k) = n, or 0
 *          if no such number exists, and d(n) = the number of divisors of n
 *          (A000005).
 * https://oeis.org/A264970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if A262686(n) = 0 otherwise a(n) = 1 + a(A262686(n))
 *            where A262686(n) = largest k such that k - d(k) = n, d(k) =
 *            number of divisors of k. The implementation pre‑computes divisor
 *            counts up to a fixed limit, builds the mapping f(n), and uses
 *            memoised recursion to compute a(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Constants: maximum indices that will be pre‑computed.               */
#define MAX_N 200000          /* highest n that can be handled          */
#define EXTRA 30000           /* safety margin for k – d(k) values     */
#define MAX_K (MAX_N + EXTRA) /* maximum k considered                  */

/* ------------------------------------------------------------------ */
/* Static storage                                                   */
static int initialized = 0;
static uint64_t *bestk_idx = NULL;   /* f(n) for n ≤ MAX_K                 */
static uint64_t *cache_idx = NULL;   /* memoised a(n) (integer form)       */
static unsigned char *known_flag = NULL; /* 1 if a(n) known                   */

/* ------------------------------------------------------------------ */
/* Allocate and initialise tables */
static void init(void)
{
    if (initialized) return;
    size_t sz = MAX_K + 1;

    bestk_idx  = (uint64_t*)calloc(sz, sizeof(uint64_t));
    cache_idx  = (uint64_t*)calloc(sz, sizeof(uint64_t));
    known_flag = (unsigned char*)calloc(sz, sizeof(unsigned char));
    if (!bestk_idx || !cache_idx || !known_flag) exit(1);

    /* divisor counts d[1..MAX_K] */
    uint32_t *divcnt = (uint32_t*)calloc(sz, sizeof(uint32_t));
    if (!divcnt) exit(1);

    for (uint64_t i = 1; i <= MAX_K; ++i) {
        for (uint64_t j = i; j <= MAX_K; j += i) {
            ++divcnt[j];
        }
    }

    for (uint64_t k = 1; k <= MAX_K; ++k) {
        uint64_t g = k - divcnt[k];
        if (g <= (uint64_t)MAX_K) {
            bestk_idx[g] = k; /* keep the largest k */
        }
    }

    free(divcnt);
    initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Integer version of a(n) using memoised recursion */
static uint64_t a_int(uint64_t n)
{
    if (!initialized) init();
    if (n > (uint64_t)MAX_K) return 0;

    size_t idx = (size_t)n;
    if (known_flag[idx]) return cache_idx[idx];

    uint64_t k = bestk_idx[idx];
    uint64_t res;
    if (k == 0) {
        res = 0;
    } else {
        res = 1 + a_int(k);
    }

    cache_idx[idx] = res;
    known_flag[idx] = 1;
    return res;
}

/* ------------------------------------------------------------------ */
/* Public function required by the build */
seq_t a(idx_t n)
{
    return (seq_t)a_int(n);
}
