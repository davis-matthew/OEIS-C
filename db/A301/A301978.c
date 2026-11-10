/*
 * A301978  Euler transform of A065958.
 * https://oeis.org/A301978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} 1/(1-x^k)^A065958(k), with 1/(1-x^k)^e =
 *            Sum_{j>=0} C(e+j-1, j) x^(k*j); the factors are multiplied in
 *            one at a time (no division of large sums)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 61, u128 n <= 146,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 965 (2026-11-08)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/gcd.h"

static seq_t A065958_a(idx_t n)
{
    if (n == 1)
        return 1;
    idx_t t = n;
    seq_t res = 1;
    for (idx_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            seq_t pp = (seq_t)p * (seq_t)p;
            seq_t term = 1;
            int first = 1;
            while (t % p == 0) {
                t /= p;
                if (first) { term = pp + 1; first = 0; }
                else term *= pp;
            }
            res *= term;
        }
    }
    if (t > 1)
        res *= (seq_t)t * (seq_t)t + 1;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    size_t N = (size_t)n;
    seq_t *b = mem_new(seq_t, N + 1);
    seq_t *cj = mem_new(seq_t, N + 1);
    b[0] = 1;
    for (size_t k = 1; k <= N; k++) {
        seq_t e = A065958_a((idx_t)k);
        size_t J = N / k;
        /* cj[j] = C(e+j-1, j), computed exactly without overflow of fitting values */
        cj[0] = 1;
        for (size_t j = 1; j <= J; j++) {
            seq_t c = cj[j - 1];
            seq_t num = e + (seq_t)(j - 1);
            seq_t jj = (seq_t)j;
            seq_t g = gcd(c, jj);
            c = c / g;
            jj = jj / g;
            cj[j] = c * (num / jj);
        }
        /* in-place multiplication, descending m */
        for (size_t m = N; m >= k; m--) {
            seq_t s = b[m];
            size_t j = 1;
            for (size_t r = m - k + 1; r >= k + 1 || j == 1; ) {
                /* r tracks m - k*(j-1) */
                (void)r;
                break;
            }
            for (j = 1; j * k <= m; j++)
                s += cj[j] * b[m - j * k];
            b[m] = s;
            if (m == k) break;
        }
    }
    seq_t res = b[N];
    mem_free(b);
    mem_free(cj);
    return res;
}
