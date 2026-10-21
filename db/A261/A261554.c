/*
 * A261554  Triangle read by rows: T(n,m) = number of partitions of n into at
 *          least m distinct parts, n>=1, m>=1.
 * https://oeis.org/A261554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,m) = sum_{k=m}^{floor((1+sqrt(1+8n))/2)} P_exact(n -
 *            k(k-1)/2 , k) * where P_exact(p,k) is the number of partitions
 *            of p into exactly k parts. * The sequence a(n) is the triangle
 *            read by rows: * a(1) = T(1,1), a(2) = T(2,1), a(3) = T(3,1),
 *            a(4) = T(3,2), … * (flattened in increasing n and, fo...
 * Range:     u8 n <= 119, u16 n <= 584, u32 n <= 3227, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifndef MAXN
#define MAXN 10000   /* default maximum n needed */
#endif

/* Pre‑compute once to answer any a(n) in O(1). */
static bool initialized = false;
static seq_t *pExact = NULL;   /* P_exact[n][k] table (flattened) */
static seq_t *values = NULL;   /* flattened triangle */
static idx_t totalTerms = 0;   /* number of terms in 'values' */
static idx_t KMAX = 0;         /* maximum number of parts needed */

/* integer square root; n <= 80001 so double is safe */
static inline idx_t int_isqrt(idx_t x) {
    return (idx_t)sqrt((double)x);
}

/* initialise tables the first time a() is called */
static void init_table(void) {
    if (initialized) return;

    /* compute global KMAX for n = MAXN */
    KMAX = (idx_t)((int_isqrt(1 + 8 * MAXN) - 1) / 2);

    /* allocate P_exact table */
    size_t pExactSize = (size_t)(MAXN + 1) * (size_t)(KMAX + 1);
    pExact = (seq_t *)malloc(pExactSize * sizeof(seq_t));
    if (!pExact) abort();
    memset(pExact, 0, pExactSize * sizeof(seq_t));

    /* base case */
    pExact[0 * (KMAX + 1) + 0] = 1;

    /* dynamic programming for P_exact */
    for (idx_t n = 1; n <= MAXN; ++n) {
        idx_t maxK = n < KMAX ? n : KMAX;
        for (idx_t k = 1; k <= maxK; ++k) {
            seq_t a = pExact[(n - 1) * (KMAX + 1) + (k - 1)];
            seq_t b = (n >= k) ? pExact[(n - k) * (KMAX + 1) + k] : 0;
            pExact[n * (KMAX + 1) + k] = a + b;
        }
    }

    /* total number of triangle entries */
    idx_t tot = 0;
    for (idx_t n = 1; n <= MAXN; ++n) {
        idx_t kmax_n = (idx_t)((int_isqrt(1 + 8 * n) - 1) / 2);
        tot += kmax_n;
    }
    totalTerms = tot;
    values = (seq_t *)malloc(totalTerms * sizeof(seq_t));
    if (!values) abort();

    /* temporary row buffer (max KMAX <= 141 for MAXN=10000) */
    enum { MAX_ROW = 150 };
    seq_t row[MAX_ROW];

    /* flatten the triangle into 'values' in correct order */
    idx_t pos = 0;
    for (idx_t n = 1; n <= MAXN; ++n) {
        idx_t kmax_n = (idx_t)((int_isqrt(1 + 8 * n) - 1) / 2);
        seq_t cumulative = 0;
        /* compute from kmax downwards and store in row[k-1] */
        for (idx_t k = kmax_n; k >= 1; --k) {
            idx_t reduced = n - (k * (k - 1)) / 2;
            cumulative += pExact[reduced * (KMAX + 1) + k];
            row[k - 1] = cumulative;
        }
        /* copy row in increasing m (k=1..kmax) */
        for (idx_t k = 1; k <= kmax_n; ++k) {
            values[pos++] = row[k - 1];
        }
    }

    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 0 */
    init_table();
    if (n > totalTerms) return 0;  /* safety check */
    return values[n - 1];
}
