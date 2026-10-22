/*
 * A367955  Number T(n,k) of partitions of [n] whose block maxima sum to k,
 *          triangle T(n,k), n>=0, n<=k<=n*(n+1)/2, read by rows.
 * https://oeis.org/A367955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 255, u32 n <= 887, u64 n <= 3439,
 *            u128 n <= 9903, big any n
 * Helpers:   row_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367955:
 * Number T(n,k) of partitions of [n] whose block maxima sum to k,
 * triangle read by rows. No closed‑form formula is known; the
 * implementation evaluates the sequence by dynamic programming. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/row_terms.h"

static seq_t *compute_row(idx_t n)
{
    if (n == 0) {
        seq_t *row = (seq_t *)malloc(sizeof(seq_t));
        if (!row) return NULL;
        row[0] = (seq_t)1;
        return row;
    }

    size_t maxSum = (size_t)((((unsigned __int128)n * (n + 1)) / 2));
    size_t dimS   = maxSum + 1;
    size_t dimL   = n + 2;               /* leaders 0 .. n+1, we start from 1 */
    size_t total  = dimL * dimS;

    seq_t *cur = (seq_t *)calloc(total, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(total, sizeof(seq_t));
    if (!cur || !nxt) return NULL;

    /* start with element n as the only leader */
    cur[1 * dimS + (size_t)n] = (seq_t)1;

    /* process elements n-1, n-2, … , 1 */
    for (idx_t x = n - 1; x >= 1; --x) {
        /* reset next array */
        memset(nxt, 0, total * sizeof(seq_t));

        for (size_t L = 1; L <= n; ++L) {
            size_t offL  = L * dimS;
            size_t offL1 = (L + 1) * dimS;
            for (size_t s = 0; s <= maxSum; ++s) {
                seq_t c = cur[offL + s];
                if (!c) continue;

                /* start a new block with maximum x */
                size_t ns = s + (size_t)x;
                if (ns <= maxSum)
                    nxt[offL1 + ns] += c;

                /* attach x to an existing block */
                nxt[offL + s] += c * (seq_t)L;
            }
        }
        /* swap cur and nxt for the next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* create the row array (indices 0 .. maxSum) */
    seq_t *row = (seq_t *)calloc(dimS, sizeof(seq_t));
    if (!row) return NULL;

    for (size_t k = n; k <= maxSum; ++k) {
        seq_t sum = 0;
        for (size_t L = 1; L <= n; ++L)
            sum += cur[L * dimS + k];
        row[k] = sum;
    }

    free(cur);
    free(nxt);
    return row;
}

/* cached rows */
static idx_t  *cum          = NULL;   /* cumulative term count up to row r */
static size_t  cumCap        = 0;
static idx_t   maxRow        = 0;      /* highest row computed */

static seq_t **T            = NULL;   /* T[r] points to the row array for r */
static size_t  Tcap          = 0;

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (!cum) {
        cumCap = 16;
        cum = (idx_t *)malloc(cumCap * sizeof(idx_t));
        if (!cum) return (seq_t)0;

        Tcap = 16;
        T = (seq_t **)malloc(Tcap * sizeof(seq_t *));
        if (!T) return (seq_t)0;

        /* row 0 */
        T[0] = compute_row(0);
        cum[0] = row_terms(0);
        maxRow = 0;
    }

    /* compute more rows until the desired index is within the cumulative range */
    while (n >= cum[maxRow]) {
        idx_t nextRow = maxRow + 1;

        /* grow storage arrays if needed */
        if (nextRow >= cumCap) {
            cumCap *= 2;
            cum = (idx_t *)realloc(cum, cumCap * sizeof(idx_t));
            if (!cum) return (seq_t)0;
        }
        if (nextRow >= Tcap) {
            Tcap *= 2;
            T = (seq_t **)realloc(T, Tcap * sizeof(seq_t *));
            if (!T) return (seq_t)0;
        }

        T[nextRow] = compute_row(nextRow);
        cum[nextRow] = cum[maxRow] + row_terms(nextRow);
        maxRow = nextRow;
    }

    /* locate the row and column of the requested index */
    idx_t prevCum = (maxRow == 0) ? 0ULL : cum[maxRow - 1];
    size_t offset = (size_t)(n - prevCum);          /* 0‑based within the row */
    size_t k      = (size_t)maxRow + offset;        /* block‑max sum */

    seq_t *row = T[maxRow];
    return row[k];
}
