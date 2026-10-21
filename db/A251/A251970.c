/*
 * A251970  Number of (n+1)X(4+1) 0..3 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A251970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 15, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = f(ceil((n+2)/2)) * f(floor((n+1)/2))
 * where f(L) is the number of L‑length chains of 5‑tuples over {0,1,2,3}
 * whose adjacent coordinate sums are non‑decreasing, and the product
 * counts the interleaved even‑index and odd‑index rows of an (n+1)×5 array.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */

/* representation of a 5‑tuple */
typedef struct { unsigned char v[5]; } row_t;

#define NUM_TUPLES (4*4*4*4*4)   /* 4⁵ = 1024 */
static size_t rowCount = 0;
static row_t rows[NUM_TUPLES];

static size_t *predStart = NULL;  /* start of predecessor list for each row */
static size_t *predFlat  = NULL;  /* concatenated predecessor indices */
static size_t predCap    = 0;

static seq_t *dpPrev = NULL;  /* DP for length L‑1 */
static seq_t *dpCur  = NULL;  /* DP for length L   */
static size_t dpCap  = 0;

static seq_t *fVals  = NULL;  /* f[0..] values */
static size_t fCap   = 0;
static size_t fSize  = 0;

/* ------------------------------------------------------------------ */
/* Initialize rows and predecessor tables only once. */
static void init_tables(void)
{
    static bool done = false;
    if (done) return;

    /* enumerate all admissible 5‑tuples */
    for (int a0 = 0; a0 < 4; ++a0)
        for (int a1 = 0; a1 < 4; ++a1)
            for (int a2 = 0; a2 < 4; ++a2)
                for (int a3 = 0; a3 < 4; ++a3)
                    for (int a4 = 0; a4 < 4; ++a4) {
                        int s0 = a0 + a1;
                        int s1 = a1 + a2;
                        int s2 = a2 + a3;
                        int s3 = a3 + a4;
                        if (s0 <= s1 && s1 <= s2 && s2 <= s3) {
                            rows[rowCount].v[0] = (unsigned char)a0;
                            rows[rowCount].v[1] = (unsigned char)a1;
                            rows[rowCount].v[2] = (unsigned char)a2;
                            rows[rowCount].v[3] = (unsigned char)a3;
                            rows[rowCount].v[4] = (unsigned char)a4;
                            ++rowCount;
                        }
                    }

    /* allocate predecessor lists */
    predCap   = rowCount * rowCount;
    predFlat  = (size_t *)malloc(predCap * sizeof(size_t));
    predStart = (size_t *)malloc((rowCount + 1) * sizeof(size_t));
    if (!predFlat || !predStart) exit(1);

    size_t offset = 0;
    for (size_t j = 0; j < rowCount; ++j) {
        predStart[j] = offset;
        for (size_t i = 0; i < rowCount; ++i) {
            bool ok = true;
            for (int k = 0; k < 5; ++k)
                if (rows[i].v[k] > rows[j].v[k]) { ok = false; break; }
            if (ok) predFlat[offset++] = i;
        }
    }
    predStart[rowCount] = offset;  /* sentinel */
    done = true;
}

/* ------------------------------------------------------------------ */
/* Ensure f[0..len] are computed.  f[0] = 1, f[1] = number of rows. */
static void ensure_f(size_t len)
{
    if (len <= fSize) return;
    init_tables();

    /* allocate DP arrays if needed */
    if (dpCap < rowCount) {
        dpPrev = (seq_t *)realloc(dpPrev, rowCount * sizeof(seq_t));
        dpCur  = (seq_t *)realloc(dpCur,  rowCount * sizeof(seq_t));
        if (!dpPrev || !dpCur) exit(1);
        dpCap = rowCount;
    }

    /* allocate the f array */
    if (fCap < len + 1) {
        seq_t *tmp = (seq_t *)realloc(fVals, (len + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        if (fVals) {
            for (size_t i = fCap; i <= len; ++i) tmp[i] = (seq_t)0;
        } else {
            for (size_t i = 0; i <= len; ++i) tmp[i] = (seq_t)0;
        }
        fVals = tmp;
        fCap  = len + 1;
    }

    if (fSize == 0) {                      /* base case */
        fVals[0] = (seq_t)1;
        for (size_t i = 0; i < rowCount; ++i) dpPrev[i] = (seq_t)1;
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < rowCount; ++i) sum += dpPrev[i];
        fVals[1] = sum;
        fSize = 1;
    }

    /* extend from current size + 1 up to len */
    while (fSize < len) {
        /* compute dpCur[j] = sum_{i <= j} dpPrev[i] */
        for (size_t j = 0; j < rowCount; ++j) {
            seq_t s = (seq_t)0;
            size_t beg = predStart[j];
            size_t end = predStart[j + 1];
            for (size_t idx = beg; idx < end; ++idx)
                s += dpPrev[predFlat[idx]];
            dpCur[j] = s;
        }
        /* total for this length */
        seq_t total = (seq_t)0;
        for (size_t j = 0; j < rowCount; ++j) total += dpCur[j];

        /* swap dp arrays for next iteration */
        seq_t *tmp = dpPrev; dpPrev = dpCur; dpCur = tmp;

        ++fSize;
        fVals[fSize] = total;
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point for the OEIS sequence. */
seq_t a(idx_t n)
{
    /* lengths of the two interleaved chains in an (n+1)×5 array */
    size_t evenLen = (size_t)(n + 2) / 2;   /* ceil((n+2)/2) */
    size_t oddLen  = (size_t)(n + 1) / 2;   /* floor((n+1)/2) */
    size_t need = evenLen > oddLen ? evenLen : oddLen;

    ensure_f(need);
    seq_t res = fVals[evenLen];
    res *= fVals[oddLen];
    return res;
}
