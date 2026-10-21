/*
 * A253009  Number of nX6 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 3 and every value
 *          increasing by 0 or 1 with every step right, diagonally se or down.
 * https://oeis.org/A253009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2030043136*n^3 - 57190121472*n^2 + 545623168640*n -
 *            1762327894140)/3 (for n>15) For n≤15 it is computed by a small
 *            dynamic‑programming enumeration
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint64_t idx_t;          /* assumed by the build */

/* ------------------------------------------------------------------ */
/* Helper structures and functions for the small (≤15) case.          */
/* ------------------------------------------------------------------ */
typedef struct {
    uint8_t v[6];
} pattern_t;

static bool compatible(const pattern_t *p, const pattern_t *q, uint32_t max)
{
    int j;
    for (j = 0; j < 6; ++j) {
        uint8_t prev = p->v[j];
        uint8_t cur  = q->v[j];
        /* vertical constraint */
        if (cur < prev || cur > prev + 1) return false;
        if (j > 0) {
            /* horizontal constraint */
            if (cur < q->v[j-1] || cur > q->v[j-1] + 1) return false;
            /* diagonal constraint */
            if (cur < p->v[j-1] || cur > p->v[j-1] + 1) return false;
        }
        /* global bound */
        if (cur > max) return false;
    }
    return true;
}

/* Enumerate all admissible 1‑× 6 patterns for given max value.       */
static size_t enumerate_patterns(uint32_t max, pattern_t *table)
{
    size_t cnt = 0;
    for (uint8_t first = 0; first <= max; ++first) {
        for (int mask = 0; mask < 32; ++mask) {
            uint8_t val = first;
            table[cnt].v[0] = val;
            bool ok = true;
            for (int step = 1; step < 6; ++step) {
                uint8_t inc = (mask >> (step-1)) & 1;
                val += inc;
                if (val > max) { ok = false; break; }
                table[cnt].v[step] = val;
            }
            if (ok) cnt++;
        }
    }
    return cnt;
}

/* Small‑n dynamic programming (n ≤ 15).                               */
static seq_t compute_small(idx_t n)
{
    /* The upper‑right corner value is n-4 (for n≥6).                */
    uint32_t vmax = (n >= 6) ? (uint32_t)(n - 4) : 2U;

    pattern_t pat[512];
    size_t patcnt = enumerate_patterns(vmax, pat);

    seq_t cnt[512] = {0};
    seq_t nxt[512] = {0};

    /* First row must start with 0.                                 */
    for (size_t i = 0; i < patcnt; ++i)
        if (pat[i].v[0] == 0) cnt[i] = 1;

    for (size_t row = 2; row <= n; ++row) {
        for (size_t i = 0; i < patcnt; ++i) nxt[i] = 0;
        for (size_t i = 0; i < patcnt; ++i) if (cnt[i]) {
            for (size_t j = 0; j < patcnt; ++j)
                if (compatible(&pat[i], &pat[j], vmax))
                    nxt[j] += cnt[i];
        }
        for (size_t i = 0; i < patcnt; ++i) cnt[i] = nxt[i];
    }

    seq_t result = 0;
    for (size_t i = 0; i < patcnt; ++i)
        if (pat[i].v[5] == vmax) result += cnt[i];
    return result;
}

/* ------------------------------------------------------------------ */
/* Constants for the cubic polynomial (for n > 15).                    */
#define C1  2030043136ULL
#define C2  57190121472ULL
#define C3  545623168640ULL
#define C4  1762327894140ULL

seq_t a(idx_t n)
{
    if (n <= 15U)
        return compute_small(n);

    __int128 N = ( __int128 )n;
    __int128 num = (__int128)C1 * N * N * N;
    num -= (__int128)C2 * N * N;
    num += (__int128)C3 * N;
    num -= (__int128)C4;

    return (seq_t)(num / 3);
}
