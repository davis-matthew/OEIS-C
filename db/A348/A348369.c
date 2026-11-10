/*
 * A348369  Number of ways A328596(n) (the reversed binary expansion is an
 *          aperiodic necklace) can be expressed as sum A328596(k) +
 *          A328596(m) with 0 < k,m < n. The cases A328596(k) + A328596(m) and
 *          A328596(m) + A328596(k) are considered equal.
 * https://oeis.org/A348369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Generate A328596 in the order its terms are indexed: by
 *            bit length, then lexicographically by the reversed (LSB-first)
 *            binary word, which must be a Lyndon word. a(n) = #{ {k,m} : 0 <
 *            k <= m < n, A328596(k) + A328596(m) = A328596(n) }, counted with
 *            sort + two pointers
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t *A348369_T = NULL;
static size_t A348369_cnt = 0, A348369_cap = 0;
static unsigned A348369_len = 0;   /* lengths fully generated so far */

/* r is a len-bit word read MSB-first (leading zeros allowed) */
static bool A348369_is_lyndon(uint64_t r, unsigned len)
{
    uint64_t mask = (len >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << len) - 1);
    for (unsigned k = 1; k < len; ++k) {
        uint64_t rot = ((r << k) | (r >> (len - k))) & mask;
        if (r >= rot) return false;
    }
    return true;
}

static uint64_t A348369_rev(uint64_t r, unsigned len)
{
    uint64_t x = 0;
    for (unsigned i = 0; i < len; ++i) {
        x = (x << 1) | ((r >> i) & 1);
    }
    return x;
}

static void A348369_push(uint64_t x)
{
    if (A348369_cnt == A348369_cap) {
        size_t nc = A348369_cap ? A348369_cap * 2 : 256;
        A348369_T = (uint64_t *)realloc(A348369_T, nc * sizeof(uint64_t));
        A348369_cap = nc;
    }
    A348369_T[A348369_cnt++] = x;
}

static void A348369_ensure(size_t n)
{
    while (A348369_cnt < n) {
        unsigned len = ++A348369_len;
        uint64_t lim = (uint64_t)1 << len;
        for (uint64_t r = 1; r < lim; ++r) {
            if (A348369_is_lyndon(r, len))
                A348369_push(A348369_rev(r, len));
        }
    }
}

static int A348369_cmp(const void *p, const void *q)
{
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    size_t N = (size_t)n;
    A348369_ensure(N);
    uint64_t target = A348369_T[N - 1];
    size_t m = N - 1;
    uint64_t *B = (uint64_t *)malloc(m * sizeof(uint64_t));
    for (size_t i = 0; i < m; ++i) B[i] = A348369_T[i];
    qsort(B, m, sizeof(uint64_t), A348369_cmp);
    size_t i = 0, j = m - 1;
    uint64_t res = 0;
    while (i <= j) {
        uint64_t s = B[i] + B[j];
        if (s == target) {
            ++res;
            ++i;
            if (j == 0) break;
            --j;
        } else if (s < target) {
            ++i;
        } else {
            if (j == 0) break;
            --j;
        }
    }
    free(B);
    return (seq_t)res;
}
