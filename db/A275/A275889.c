/*
 * A275889  First differences of A199134.
 * https://oeis.org/A275889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A199134(n+1) - A199134(n), where A199134 lists the rows
 *            whose greedy queen lies below the main diagonal (A065188 greedy
 *            queens simulated directly)
 * Range:     u8 n <= 19097, u16 n <= 19097, u32 n <= 19097, u64 n <= 19097,
 *            u128 n <= 19097, big any n
 * Validated: standard, pass, 117 terms through n = 9566 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define A275889_MAXC 400000
#define A275889_DOFF A275889_MAXC
#define A275889_MAXD (2 * A275889_MAXC + 2)

static uint8_t  A275889_used[A275889_MAXC + 2];
static uint8_t  A275889_d1[A275889_MAXD];
static uint8_t  A275889_d2[A275889_MAXD];
static uint64_t A275889_rows = 0;
static uint64_t A275889_minfree = 1;
static uint64_t *A275889_below = NULL;
static size_t A275889_cap = 0, A275889_len = 0;

static void A275889_push(uint64_t r)
{
    if (A275889_len >= A275889_cap) {
        size_t nc = A275889_cap ? A275889_cap * 2 : 1024;
        uint64_t *t = (uint64_t *)realloc(A275889_below, nc * sizeof(uint64_t));
        if (!t) exit(EXIT_FAILURE);
        A275889_below = t;
        A275889_cap = nc;
    }
    A275889_below[A275889_len++] = r;
}

static void A275889_fill(size_t need)
{
    while (A275889_len < need) {
        uint64_t r = ++A275889_rows;
        uint64_t c = A275889_minfree;
        for (;;) {
            if (c > A275889_MAXC) exit(EXIT_FAILURE);
            if (!A275889_used[c]) {
                size_t i1 = (size_t)(r + A275889_DOFF - c);
                size_t i2 = (size_t)(r + c);
                if (i1 >= A275889_MAXD || i2 >= A275889_MAXD) exit(EXIT_FAILURE);
                if (!A275889_d1[i1] && !A275889_d2[i2]) break;
            }
            ++c;
        }
        A275889_used[c] = 1;
        A275889_d1[(size_t)(r + A275889_DOFF - c)] = 1;
        A275889_d2[(size_t)(r + c)] = 1;
        while (A275889_used[A275889_minfree]) ++A275889_minfree;
        if (c < r) A275889_push(r);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    A275889_fill((size_t)n + 1);
    return (seq_t)(A275889_below[n] - A275889_below[n - 1]);
}
