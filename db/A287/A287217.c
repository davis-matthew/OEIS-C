/*
 * A287217  Primes p where A196202 reaches a new local minimum, i.e., the
 *          value of A196202 is strictly smaller than the value for both the
 *          previous prime and the next prime.
 * https://oeis.org/A287217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p(k) with A196202(k) < A196202(k-1) and
 *            A196202(k) < A196202(k+1), where A196202(k) = 2^(prime(k)-1) mod
 *            prime(k)^2; computed by a sieve of Eratosthenes, doubling the
 *            limit until n terms are found
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t a287217_w(uint64_t p)
{
    unsigned __int128 m = (unsigned __int128)p * p;
    unsigned __int128 r = 1, b = 2 % m;
    uint64_t e = p - 1;
    while (e) {
        if (e & 1) r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return (uint64_t)r;
}

seq_t a(idx_t n)
{
    uint64_t limit = 2000;
    if (n == 0) return (seq_t)0;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
        uint64_t i, j, count = 0;
        uint64_t pprev = 0, pcur = 0;
        uint64_t vprev = 0, vcur = 0;
        if (!comp) return (seq_t)0;
        for (i = 2; i * i <= limit; i++)
            if (!comp[i])
                for (j = i * i; j <= limit; j += i)
                    comp[j] = 1;
        for (i = 2; i <= limit; i++) {
            uint64_t v;
            if (comp[i]) continue;
            v = a287217_w(i);
            /* now pprev, pcur, i are three consecutive primes */
            if (pprev != 0 && vcur < vprev && vcur < v) {
                count++;
                if (count == (uint64_t)n) {
                    free(comp);
                    return (seq_t)pcur;
                }
            }
            pprev = pcur; vprev = vcur;
            pcur = i; vcur = v;
        }
        free(comp);
        limit *= 2;
    }
}
