/*
 * A354455  a(n) is the first composite number in the n-th row of A328739.
 * https://oeis.org/A354455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Row n of A328739 starts with the first n+1 primes. Each
 *            later term is the smallest number larger than the previous term
 *            that is coprime to the previous n terms. a(n) is the first
 *            composite term produced this way
 * Range:     u8 n <= 51, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static bool A354455_isprime(seq_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    seq_t d = 3;
    while (d <= x / d) {
        if (x % d == 0) return false;
        d += 2;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t w = n + 1;
    seq_t *buf = mem_new(seq_t, w);
    seq_t c = 1;
    for (idx_t i = 0; i < w; ++i) {
        seq_t prev = c;
        c++;
        while (!A354455_isprime(c)) {
            c++;
            if (c <= prev) { mem_free(buf); return 0; } /* overflow */
        }
        if (c <= prev) { mem_free(buf); return 0; }
        buf[i] = c;
    }
    seq_t last = buf[n];
    idx_t k = w;
    for (;;) {
        seq_t cand = last + 1;
        if (cand <= last) { mem_free(buf); return 0; } /* overflow */
        for (;;) {
            bool ok = true;
            for (idx_t t = 1; t <= n; ++t) {
                if (gcd(cand, buf[(k - t) % w]) != 1) { ok = false; break; }
            }
            if (ok) break;
            cand++;
            if (cand <= last) { mem_free(buf); return 0; } /* overflow */
        }
        if (!A354455_isprime(cand)) {
            mem_free(buf);
            return cand;
        }
        buf[k % w] = cand;
        last = cand;
        k++;
    }
}
