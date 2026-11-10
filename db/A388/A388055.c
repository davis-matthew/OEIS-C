/*
 * A388055  The binary expansion of the n-th term of A074832, when read
 *          backwards, corresponds to that of the a(n)-th term of A074832.
 * https://oeis.org/A388055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A074832(n) = n-th prime p whose binary reversal is also prime;
 *            a(n) = index of the binary reversal of A074832(n) in A074832
 *            (computed by sieve and count)
 * Range:     u8 n <= 174, u16 n <= 11759, u32 n <= 11759, u64 n <= 11759,
 *            u128 n <= 11759, big any n
 * Validated: standard, pass, 83 terms through n = 9941 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A388055_rev(uint64_t p)
{
    uint64_t r = 0;
    while (p > 0) {
        r = (r << 1) | (p & 1);
        p >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    unsigned L;
    for (L = 2; L < 40; L++) {
        uint64_t lim = ((uint64_t)1 << L);
        unsigned char *comp = (unsigned char *)calloc((size_t)lim, 1);
        uint64_t i, j, cnt = 0, p = 0;
        if (!comp) return 0;
        comp[0] = 1;
        comp[1] = 1;
        for (i = 2; i * i < lim; i++) {
            if (!comp[i]) {
                for (j = i * i; j < lim; j += i) comp[j] = 1;
            }
        }
        /* count qualifying primes below 2^L */
        for (i = 2; i < lim; i++) {
            if (!comp[i] && !comp[A388055_rev(i)]) cnt++;
        }
        if (cnt >= (uint64_t)n) {
            uint64_t k = 0, r, res = 0;
            for (i = 2; i < lim; i++) {
                if (!comp[i] && !comp[A388055_rev(i)]) {
                    k++;
                    if (k == (uint64_t)n) { p = i; break; }
                }
            }
            r = A388055_rev(p);
            for (i = 2; i <= r; i++) {
                if (!comp[i] && !comp[A388055_rev(i)]) res++;
            }
            free(comp);
            return (seq_t)res;
        }
        free(comp);
    }
    return 0;
}
