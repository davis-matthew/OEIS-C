/*
 * A356921  Irregular table read by rows: necklaces on the alphabet {0,1}
 *          sorted by length then by lexicographic order of minimum rotation.
 * https://oeis.org/A356921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n-th bit in the concatenation of all binary necklaces
 *            sorted by length then by lexicographic order of the minimal
 *            rotation. Each necklace of length L contributes L bits
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   db_generate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/db_generate.h"

/* maximum length of necklaces generated; enough for n up to ≈ 2·10⁶ bits */
#define MAX_LEN 20

/* structure to hold pre‑computed data for each length */
typedef struct {
    size_t count;      /* number of necklaces of this length   */
    size_t bits_len;   /* total number of bits (count * length) */
    char *bits;        /* concatenated bits of all necklaces   */
} RowData;

static RowData rows[MAX_LEN + 1];
static size_t cum_bits[MAX_LEN + 1];
static unsigned int phi[MAX_LEN + 1];
static bool initialized = false;

/* ------------------------------------------------------------- */
/* 1. Euler’s phi up to MAX_LEN                                  */
static void compute_phi(void)
{
    for (unsigned int i = 0; i <= MAX_LEN; ++i) phi[i] = i;
    for (unsigned int p = 2; p <= MAX_LEN; ++p) {
        if (phi[p] == p) { /* p is prime */
            for (unsigned int m = p; m <= MAX_LEN; m += p)
                phi[m] -= phi[m] / p;
        }
    }
}

/* ------------------------------------------------------------- */
/* 2. Recursive necklace generator (Knuth algorithm)             */

/* ------------------------------------------------------------- */
/* 3. Pre‑computation of rows up to MAX_LEN                      */
static void precompute_all(void)
{
    compute_phi();
    cum_bits[0] = 0;

    for (int L = 1; L <= MAX_LEN; ++L) {
        /* number of necklaces of length L */
        uint64_t sum = 0;
        for (int d = 1; d <= L; ++d) {
            if (L % d == 0) {
                sum += (uint64_t)phi[d] * (1ULL << (L / d));
            }
        }
        size_t N = (size_t)(sum / L);               /* number of necklaces */

        size_t bits_needed = N * (size_t)L;
        rows[L].bits = (char *)malloc(bits_needed);
        if (!rows[L].bits) abort();

        int *a = (int *)malloc((L + 1) * sizeof(int));
        if (!a) abort();
        a[0] = 0;
        size_t pos = 0;
        db_generate(1, 1, L, a, rows[L].bits, &pos);
        free(a);

        rows[L].count = N;
        rows[L].bits_len = pos;          /* should equal bits_needed */
        cum_bits[L] = cum_bits[L - 1] + bits_needed;
    }

    initialized = true;
}

/* ------------------------------------------------------------- */
/* 4. Main sequence function                                     */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* values below offset are undefined */
    if (!initialized) precompute_all();

    /* find the row length that contains position n */
    size_t L = 1;
    while (L <= MAX_LEN && cum_bits[L] < (size_t)n) ++L;
    if (L > MAX_LEN) return (seq_t)0;   /* should not happen for tested n */

    size_t prev = cum_bits[L - 1];
    size_t offset = (size_t)n - prev - 1;      /* zero‑based within row */
    size_t j = offset / (size_t)L;            /* which necklace (0‑based) */
    size_t b = offset % (size_t)L;            /* bit position in that necklace */

    char bit = rows[L].bits[j * L + b];
    return (seq_t)bit;
}
