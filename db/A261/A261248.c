/*
 * A261248  Positive fundamental solution y(n) of the generalized Pell
 *          equation X^2 - D(n) Y(n) = 2 with D(n) = A261246(n).
 * https://oeis.org/A261248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y, the least positive y with x^2 - D*y^2 = 2, where D =
 *            A261246(n); found by a search over D using the continued
 *            fraction of sqrt(D), where d_{k+1} = 2 at odd k gives a solution
 *            with y = q_k
 * Range:     u8 n <= 17, u16 n <= 34, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

static uint64_t A261248_isqrt(uint64_t D)
{
    uint64_t r = (uint64_t)sqrt((double)D);
    while (r * r > D) --r;
    while ((r + 1) * (r + 1) <= D) ++r;
    return r;
}

/* Returns true if x^2 - D y^2 = 2 is soluble; *y gets the smallest y. */
static bool A261248_solve(uint64_t D, seq_t *y)
{
    if (D == 2) { *y = 1; return true; }
    uint64_t a0 = A261248_isqrt(D);
    if (a0 * a0 == D) return false;
    uint64_t m = 0, d = 1, a = a0;
    seq_t q_prev = 0, q = 1;
    uint64_t k = 0;
    while (true) {
        m = d * a - m;
        d = (D - m * m) / d;           /* d_{k+1} */
        if ((k & 1) && d == 2) { *y = q; return true; }
        if (d == 1 && (((k + 1) & 1) == 0)) return false;
        a = (a0 + m) / d;
        seq_t qn = (seq_t)a * q + q_prev;
        q_prev = q;
        q = qn;
        ++k;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t next_D = 2;

    while (len < (size_t)n) {
        seq_t y = 0;
        while (!A261248_solve(next_D, &y))
            ++next_D;
        ++next_D;
        if (len == cap) {
            size_t nc = cap ? cap * 2 : 64;
            if (cache == NULL) cache = mem_new(seq_t, nc);
            else cache = mem_resize(seq_t, cache, nc);
            cap = nc;
        }
        cache[len++] = y;
    }
    return cache[n - 1];
}
