/*
 * A366885  Dedekind psi function applied to the odd part of n, permuted by
 *          A163511: a(n) = A347385(A163511(n)).
 * https://oeis.org/A366885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A347385(A163511(n)), where A163511(n) = p(r) *
 *            Product_{m=1..r} p(m)^A163510(n,m) with r = A000120(n); A347385
 *            is multiplicative with a(2^e) = 1 and a(p^e) = (p+1)*p^(e-1) for
 *            odd primes p
 * Range:     u8 n <= 32, u16 n <= 258, u32 n <= 8206, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 92 terms through n = 9533 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* first 64 primes by trial division */
    static uint32_t pr[64];
    static int init = 0;
    if (!init) {
        int c = 0;
        uint32_t x = 2;
        while (c < 64) {
            bool ok = true;
            for (int j = 0; j < c && (uint32_t)pr[j] * pr[j] <= x; j++) {
                if (x % pr[j] == 0) { ok = false; break; }
            }
            if (ok) pr[c++] = x;
            x++;
        }
        init = 1;
    }

    int r = 0;
    for (idx_t t = n; t; t >>= 1) if (t & 1) r++;

    seq_t res = 1;
    int i = 0;
    int prev = 0;
    int bitpos = 1;
    for (idx_t t = n; t; t >>= 1, bitpos++) {
        if (!(t & 1)) continue;
        int e = bitpos - prev - 1;          /* exponent of p(i+1) from A163510 */
        if (i == r - 1) e += 1;             /* extra factor p(r) */
        prev = bitpos;
        if (i >= 1 && e > 0) {              /* skip prime 2 (odd part) */
            seq_t p = (seq_t)pr[i];
            seq_t c = p + 1;
            for (int k = 1; k < e; k++) c *= p;
            res *= c;
        }
        i++;
    }
    return res;
}
