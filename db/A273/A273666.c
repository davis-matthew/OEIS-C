/*
 * A273666  Permutation of nonnegative integers: a(0) = 0; after which, a(2n)
 *          = A153880(a(n)), a(2n+1) = A273670(a(n)).
 * https://oeis.org/A273666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(0) = 0; a(2n) = A153880(a(n)), a(2n+1) = A273670(a(n));
 *            computed by applying the maps along the binary digits of n from
 *            the top. A273670(m) is found by binary search using a
 *            factorial-base digit count
 * Range:     u8 n <= 31, u16 n <= 255, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 96 terms through n = 977 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t A153880_a(seq_t n)
{
    seq_t result = 0;
    seq_t fac = 2;
    seq_t i = 1;
    while (n > 0) {
        seq_t d = n % (i + 1);
        result += d * fac;
        n = (n - d) / (i + 1);
        fac *= (i + 2);
        ++i;
    }
    return result;
}

/* number of y in [0,x] with no maximal factorial-base digit */
static seq_t A273666_nonmax(seq_t x)
{
    seq_t g = 1;        /* g_0 */
    seq_t fact = 1;     /* (j-1)! */
    seq_t j = 1;
    while (x > 0) {
        seq_t d = x % (j + 1);
        x = x / (j + 1);
        seq_t ng = d * fact;
        if (d != j) ng += g;
        g = ng;
        fact *= j;
        ++j;
    }
    return g;
}

/* count of numbers in [0,x] having a maximal digit */
static seq_t A273666_f(seq_t x)
{
    return x + 1 - A273666_nonmax(x);
}

/* A273670(m), offset 0, a(0)=1 */
static seq_t A273670_a(seq_t m)
{
    seq_t target = m + 1;
    seq_t hi = 1;
    while (A273666_f(hi) < target) {
        seq_t h2 = hi << 1;
        if (h2 < hi) break;
        hi = h2;
    }
    seq_t lo = 0;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (A273666_f(mid) >= target) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t x = 0;
    if (n == 0) return x;
    int top = 63;
    while (!((n >> top) & 1)) --top;
    for (int b = top; b >= 0; --b) {
        if ((n >> b) & 1) x = A273670_a(x);
        else x = A153880_a(x);
    }
    return x;
}
