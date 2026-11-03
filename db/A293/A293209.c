/*
 * A293209  Let b be the lexicographically earliest sequence of positive terms
 *          such that the function f defined by f(n) = Sum_{k=1..n} (i^k *
 *          b(k)) for any n >= 0 is injective (where i denotes the imaginary
 *          unit), and b(n) != b(n+1) and b() != b(n+2) for any n > 0; a(n) =
 *          imaginary part of f(n).
 * https://oeis.org/A293209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = imaginary part of f(n) where f(n) = Sum_{k=1..n} (i^k *
 *            b(k)), and b(k) is the lexicographically earliest positive
 *            sequence with f(n) injective and b(k) != b(k+1) and b(k) !=
 *            b(k+2)
 * Range:     i8 n <= 59999, i16 n <= 59999, i32 n <= 59999, i64 n <= 59999,
 *            i128 n <= 59999, big any n
 * Validated: standard, pass, 88 terms through n = 9228 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* f(k) = f(k-1) + i^k b(k) walks the Gaussian integers; b(k) is the least
   positive value different from b(k-1) and b(k-2) whose f(k) is a new
   point, and a(n) is the imaginary part of f(n). The walk
   is built in a loop, with growing arrays and an open-addressing hash set of
   the points visited. (The original recursed once per term, overflowing the
   stack, searched all points for each candidate, and had fixed buffers for
   n <= 20000.) The coordinates stay small, so 64 bits hold them. */

#include <stdint.h>
#include <stdlib.h>

static int64_t *B = NULL, *Re = NULL, *Im = NULL;   /* b(k) and f(k) */
static size_t len = 0, cap = 0;                     /* f(0..len) known */

static uint64_t *hkey = NULL;                       /* the set of points; 0 = empty slot */
static size_t hcap = 0, hcnt = 0;

static uint64_t key(int64_t r, int64_t i)           /* never 0 for |r|, |i| < 2^30 */
{
    return ((uint64_t)(uint32_t)(r + 0x40000000) << 32) | (uint32_t)(i + 0x40000000);
}

static size_t slot(uint64_t k)
{
    uint64_t h = k * 0x9E3779B97F4A7C15ull;
    size_t s = (size_t)(h >> 20) & (hcap - 1);
    while (hkey[s] && hkey[s] != k) s = (s + 1) & (hcap - 1);
    return s;
}

static void insert(uint64_t k)
{
    if (2 * (hcnt + 1) > hcap) {                    /* keep the load under 1/2 */
        size_t oc = hcap;
        uint64_t *old = hkey;
        hcap = hcap ? 2 * hcap : 1024;
        hkey = (uint64_t *)calloc(hcap, sizeof *hkey);
        if (!hkey) abort();
        for (size_t j = 0; j < oc; ++j)
            if (old[j]) hkey[slot(old[j])] = old[j];
        free(old);
    }
    size_t s = slot(k);
    if (!hkey[s]) { hkey[s] = k; ++hcnt; }
}

static int seen(int64_t r, int64_t i)
{
    return hkey[slot(key(r, i))] != 0;
}

static void grow(size_t need)
{
    if (need <= cap) return;
    size_t c = cap ? cap : 1024;
    while (c < need) c *= 2;
    int64_t *b = (int64_t *)realloc(B, c * sizeof *b);
    if (!b) abort();
    B = b;
    int64_t *re = (int64_t *)realloc(Re, c * sizeof *re);
    if (!re) abort();
    Re = re;
    int64_t *im = (int64_t *)realloc(Im, c * sizeof *im);
    if (!im) abort();
    Im = im;
    cap = c;
}

seq_t a(idx_t n)
{
    if (!cap) {
        grow(1);
        B[0] = Re[0] = Im[0] = 0;
        insert(key(0, 0));
        len = 0;
    }
    grow((size_t)n + 1);
    while (len < (size_t)n) {
        size_t k = len + 1;
        for (int64_t c = 1;; ++c) {
            if (c == B[k - 1]) continue;
            if (k >= 2 && c == B[k - 2]) continue;
            int64_t r = Re[k - 1], i = Im[k - 1];
            switch (k & 3) {
            case 0: r += c; break;                  /* i^0 */
            case 1: i += c; break;                  /* i^1 */
            case 2: r -= c; break;                  /* i^2 */
            case 3: i -= c; break;                  /* i^3 */
            }
            if (seen(r, i)) continue;
            B[k] = c;
            Re[k] = r;
            Im[k] = i;
            insert(key(r, i));
            break;
        }
        len = k;
    }
    return (seq_t)Im[n];
}
