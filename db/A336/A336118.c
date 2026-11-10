/*
 * A336118  a(n) = A335904(n) - A087436(n).
 * https://oeis.org/A336118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A335904(n) - A087436(n), totally additive; a(2) = 0,
 *            a(odd prime p) = a((p-1)/2) + a((p+1)/2) + A087436((p-1)/2) +
 *            A087436((p+1)/2)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9533 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A336118_CACHE_N 131072

static seq_t *A336118_cache = NULL;
static uint8_t A336118_done[A336118_CACHE_N];

/* number of odd prime factors with multiplicity */
static seq_t A336118_oddomega(idx_t n)
{
    if (n <= 1) return 0;
    idx_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;
    seq_t cnt = 0;
    for (idx_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) { m /= p; ++cnt; }
    }
    if (m > 1) ++cnt;
    return cnt;
}

static seq_t A336118_b(idx_t n)
{
    if (n <= 1) return 0;
    if (n < A336118_CACHE_N) {
        if (A336118_cache == NULL) A336118_cache = mem_new(seq_t, A336118_CACHE_N);
        if (A336118_done[n]) return A336118_cache[n];
    }
    seq_t val = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;
    bool found = false;
    for (idx_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) {
            m /= p;
            val += A336118_b(p);
            found = true;
        }
    }
    if (m > 1) {
        if (!found && m == n) {
            /* n is an odd prime */
            idx_t l = (n - 1) / 2, r = (n + 1) / 2;
            val = A336118_b(l) + A336118_b(r) + A336118_oddomega(l) + A336118_oddomega(r);
        } else {
            val += A336118_b(m);
        }
    }
    if (n < A336118_CACHE_N) {
        A336118_cache[n] = val;
        A336118_done[n] = 1;
    }
    return val;
}

seq_t a(idx_t n)
{
    return A336118_b(n);
}
