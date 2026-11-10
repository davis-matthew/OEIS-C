/*
 * A261621  Rocket Sequence 42: a(0) = 42, a(n) = A073846(a(n-1)).
 * https://oeis.org/A261621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 42, a(n) = A073846(a(n-1)); A073846(1)=1,
 *            A073846(2k)=prime(k), A073846(2k+1)=k-th composite
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 54 (2026-11-07)
 * Helpers:   mem, nth_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/nth_prime.h"
#include "../utils/prime_pi.h"

static seq_t *r261621_cache = NULL;
static idx_t r261621_have = 0;   /* number of cached terms */
static idx_t r261621_cap = 0;

/* k-th composite (k>=1): smallest c with c-1-pi(c) = k */
static uint64_t r261621_kth_composite(uint64_t k)
{
    uint64_t c = k + 1;
    for (;;) {
        uint64_t p = (uint64_t)prime_pi((idx_t)c);
        uint64_t nc = k + 1 + p;
        if (nc == c) break;
        c = nc;
    }
    return c;
}

/* A073846 applied to m */
static seq_t r261621_step(seq_t m)
{
    uint64_t v = (uint64_t)m;
    if (v <= 1) return (seq_t)1;
    if ((v & 1) == 0)
        return (seq_t)nth_prime((idx_t)(v / 2));
    return (seq_t)r261621_kth_composite((v - 1) / 2);
}

seq_t a(idx_t n)
{
    if (r261621_cap == 0) {
        r261621_cap = 128;
        r261621_cache = mem_new(seq_t, r261621_cap);
        r261621_cache[0] = (seq_t)42;
        r261621_have = 1;
    }
    if (n >= r261621_cap) {
        idx_t nc = r261621_cap;
        while (nc <= n) nc *= 2;
        r261621_cache = mem_resize(seq_t, r261621_cache, nc);
        r261621_cap = nc;
    }
    while (r261621_have <= n) {
        r261621_cache[r261621_have] = r261621_step(r261621_cache[r261621_have - 1]);
        r261621_have++;
    }
    return r261621_cache[n];
}
