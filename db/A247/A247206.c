/*
 * A247206  a(n) = round(prime(n)/A245085(n)).
 * https://oeis.org/A247206
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(prime(n)/A245085(n)), computed by direct search for
 *            A245085 (factorial residues mod prime(n)); round half up
 * Range:     u8 n <= 400, u16 n <= 2002, u32 n <= 2002, u64 n <= 2002,
 *            u128 n <= 2002, big any n
 * Validated: standard, pass, 90 terms through n = 1982 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t A247206_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
    }
    while (sz < (size_t)k) {
        if (sz == 0) {
            pr[sz++] = 2;
            continue;
        }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t q = pr[i];
            if ((uint64_t)q * q > (uint64_t)p) break;
            if (p % q == 0) { ok = false; break; }
        }
        if (!ok) { cand += 2; continue; }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }
    return pr[(size_t)k - 1];
}

static uint64_t A247206_t(uint32_t p)
{
    uint8_t *seen = (uint8_t *)calloc((size_t)p, 1);
    if (!seen) return 1;
    uint64_t fact = 1, t = 0;
    for (uint64_t k = 2; k < p; ++k) {
        fact = (fact * k) % p;
        if (seen[fact]) { t = k - 1; break; }
        seen[fact] = 1;
    }
    if (t == 0) t = (uint64_t)p - 1;
    free(seen);
    return t;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;
    uint32_t p = A247206_get_prime(n);
    uint64_t t = A247206_t(p);
    uint64_t r = (2 * (uint64_t)p + t) / (2 * t);
    return (seq_t)r;
}
