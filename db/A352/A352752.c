/*
 * A352752  a(n) is the smallest nonnegative number that requires n
 *          applications of the modified Sisyphus function of order 3, x ->
 *          A350709(x) to reach any number in the cycle {4031, 4112, 4220}.
 * https://oeis.org/A352752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   search. Let f = A350709. f(x) depends only on the signature
 *            (c0,c1,c2) of x, where ck is the number of digits of x congruent
 *            to k mod 3. So every x with a given signature has depth 1 +
 *            depth(f-value), unless x is in the cycle. The code tries digit
 *            lengths d = 1, 2, ... in turn. For each signature...
 * Range:     u8 none, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 4 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A352752_in_cycle(uint64_t v)
{
    return v == 4031 || v == 4112 || v == 4220;
}

static uint64_t A352752_concat(uint64_t x, uint64_t y)
{
    uint64_t p = 10;
    while (p <= y) p *= 10;
    return x * p + y;
}

static uint64_t A352752_f_sig(uint64_t d, uint64_t c0, uint64_t c1, uint64_t c2)
{
    uint64_t r = d;
    r = A352752_concat(r, c0);
    r = A352752_concat(r, c1);
    r = A352752_concat(r, c2);
    return r;
}

static uint64_t A352752_f(uint64_t v)
{
    uint64_t c[3] = {0, 0, 0}, d = 0;
    if (v == 0) { d = 1; c[0] = 1; }
    while (v > 0) { c[(v % 10) % 3]++; d++; v /= 10; }
    return A352752_f_sig(d, c[0], c[1], c[2]);
}

static long A352752_depth(uint64_t v)
{
    long s = 0;
    while (!A352752_in_cycle(v))
    {
        v = A352752_f(v);
        if (++s > 1000) return -1;
    }
    return s;
}

/* smallest number with digit-class counts c0,c1,c2 (d = c0+c1+c2 >= 1) */
static seq_t A352752_minnum(int c0, int c1, int c2)
{
    seq_t r;
    int i;
    if (c0 + c1 + c2 == 1)
        return (seq_t)(c0 ? 0 : (c1 ? 1 : 2));
    if (c1 > 0) { r = 1; c1--; }
    else if (c2 > 0) { r = 2; c2--; }
    else { r = 3; c0--; }
    for (i = 0; i < c0; i++) r = r * 10;
    for (i = 0; i < c1; i++) r = r * 10 + 1;
    for (i = 0; i < c2; i++) r = r * 10 + 2;
    return r;
}

static uint64_t A352752_minnum_u64(int c0, int c1, int c2)
{
    uint64_t r;
    int i;
    if (c0 + c1 + c2 == 1)
        return (uint64_t)(c0 ? 0 : (c1 ? 1 : 2));
    if (c1 > 0) { r = 1; c1--; }
    else if (c2 > 0) { r = 2; c2--; }
    else { r = 3; c0--; }
    for (i = 0; i < c0; i++) r = r * 10;
    for (i = 0; i < c1; i++) r = r * 10 + 1;
    for (i = 0; i < c2; i++) r = r * 10 + 2;
    return r;
}

seq_t a(idx_t n)
{
    int d, c0, c1, c2;
    if (n == 0)
    {
        uint64_t m = 4031;
        if (4112 < m) m = 4112;
        if (4220 < m) m = 4220;
        return (seq_t)m;
    }
    for (d = 1; d <= 200; d++)
    {
        int found = 0;
        seq_t best = 0;
        for (c0 = 0; c0 <= d; c0++)
            for (c1 = 0; c0 + c1 <= d; c1++)
            {
                long dep;
                c2 = d - c0 - c1;
                if (d <= 18)
                    dep = A352752_depth(A352752_minnum_u64(c0, c1, c2));
                else
                {
                    long t = A352752_depth(A352752_f_sig((uint64_t)d, (uint64_t)c0,
                                                         (uint64_t)c1, (uint64_t)c2));
                    dep = (t < 0) ? -1 : t + 1;
                }
                if (dep >= 0 && (idx_t)dep == n)
                {
                    seq_t m = A352752_minnum(c0, c1, c2);
                    if (!found || m < best) { best = m; found = 1; }
                }
            }
        if (found) return best;
    }
    return (seq_t)0;
}
