/*
 * A248129  The limiting sequence of terms preceding the 0's in A248128.
 * https://oeis.org/A248129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A248128(m-n) where A248128(m)=0 and A248128(m-k)>0 for
 *            0<k<=n; A248128(0)=50, A248128(n)=3*(n-th digit of the digit
 *            stream of the sequence). Computed by generating A248128 until a
 *            0 is preceded by a positive run longer than n
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static void A248129_gen(unsigned char *terms, unsigned char *digs, size_t L)
{
    size_t dl = 2, pos = 0, tl = 1;
    terms[0] = 50;
    digs[0] = 5;
    digs[1] = 0;
    while (tl < L) {
        unsigned char t = (unsigned char)(3 * digs[pos++]);
        terms[tl++] = t;
        if (t >= 10) {
            digs[dl++] = (unsigned char)(t / 10);
            digs[dl++] = (unsigned char)(t % 10);
        } else {
            digs[dl++] = t;
        }
    }
}

seq_t a(idx_t n)
{
    size_t L = 1024;
    size_t nn = (size_t)n;
    for (;;) {
        unsigned char *terms = (unsigned char *)malloc(L);
        unsigned char *digs = (unsigned char *)malloc(2 * L + 4);
        A248129_gen(terms, digs, L);
        /* find last zero m and previous zero p */
        long long m = -1, p = -1;
        for (long long i = (long long)L - 1; i >= 1; --i) {
            if (terms[i] == 0) {
                if (m < 0) m = i;
                else { p = i; break; }
            }
        }
        if (m >= 0 && p >= 0 && (size_t)(m - p - 1) >= nn) {
            unsigned char v = terms[(size_t)m - nn];
            free(terms);
            free(digs);
            return (seq_t)v;
        }
        free(terms);
        free(digs);
        L *= 2;
    }
}
