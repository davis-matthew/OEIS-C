/*
 * A272465  Indices n such that A272464(n)=1.
 * https://oeis.org/A272465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Rows are the runs of A272464 that
 *            start at each 1. Row 1 = [1]. Row r+1 = x_1, [x_1+x_2], x_2,
 *            ..., x_L, [x_L+1], where x_1..x_L is row r and each bracketed
 *            sum is kept only if that value has not appeared anywhere before.
 *            a(1) = 1 and a(r+1) = a(r) + (length of row r)
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t *A272465_row = NULL;    /* current row R */
static size_t A272465_rowlen = 0;
static uint64_t *A272465_start = NULL;  /* A272465_start[r] = index of the r-th 1 */
static size_t A272465_scap = 0;
static size_t A272465_R = 0;            /* rows known: start[1..R]; A272465_row holds row R */
static uint8_t *A272465_bits = NULL;    /* seen-value bitset */
static uint64_t A272465_bitcap = 0;     /* number of bits allocated */

static int A272465_seen(uint64_t v)
{
    if (v >= A272465_bitcap) return 0;
    return (A272465_bits[v >> 3] >> (v & 7)) & 1;
}

static void A272465_mark(uint64_t v)
{
    if (v >= A272465_bitcap) {
        uint64_t nc = A272465_bitcap ? A272465_bitcap : ((uint64_t)1 << 16);
        while (nc <= v) nc <<= 1;
        A272465_bits = (uint8_t *)realloc(A272465_bits, (size_t)(nc >> 3));
        for (uint64_t i = A272465_bitcap >> 3; i < (nc >> 3); i++) A272465_bits[i] = 0;
        A272465_bitcap = nc;
    }
    A272465_bits[v >> 3] |= (uint8_t)(1u << (v & 7));
}

static void A272465_next_row(void)
{
    size_t L = A272465_rowlen;
    uint64_t *nx = (uint64_t *)malloc((2 * L + 1) * sizeof(uint64_t));
    size_t m = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t x = A272465_row[i];
        uint64_t y = (i + 1 < L) ? A272465_row[i + 1] : 1;
        uint64_t s = x + y;
        nx[m++] = x;
        if (!A272465_seen(s)) {
            A272465_mark(s);
            nx[m++] = s;
        }
    }
    free(A272465_row);
    A272465_row = (uint64_t *)realloc(nx, (m ? m : 1) * sizeof(uint64_t));
    A272465_rowlen = m;
}

seq_t a(idx_t n)
{
    if (A272465_R == 0) {
        A272465_row = (uint64_t *)malloc(sizeof(uint64_t));
        A272465_row[0] = 1;
        A272465_rowlen = 1;
        A272465_mark(1);
        A272465_scap = 64;
        A272465_start = (uint64_t *)malloc(A272465_scap * sizeof(uint64_t));
        A272465_start[1] = 1;
        A272465_R = 1;
    }
    while (A272465_R < (size_t)n) {
        /* start[R+1] = start[R] + len(row R) */
        if (A272465_R + 2 > A272465_scap) {
            A272465_scap *= 2;
            A272465_start = (uint64_t *)realloc(A272465_start, A272465_scap * sizeof(uint64_t));
        }
        A272465_start[A272465_R + 1] = A272465_start[A272465_R] + (uint64_t)A272465_rowlen;
        A272465_R++;
        if (A272465_R < (size_t)n) A272465_next_row();
        else {
            /* row R not yet built; build lazily only when the next start is requested */
            A272465_next_row();
        }
    }
    return (seq_t)A272465_start[n];
}
