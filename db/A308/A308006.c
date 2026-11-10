/*
 * A308006  Iterate the map x->A308005(x) starting at x=n; a(n) is the number
 *          of steps before the first repeated term is encountered.
 * https://oeis.org/A308006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (index of first repeated term in the orbit of n under x
 *            -> A308005(x)) - 1; computed by iterating and comparing against
 *            earlier terms
 * Range:     u8 n <= 524, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9068 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A308006_shift(seq_t acc, unsigned v)
{
    seq_t p = 10;
    while ((seq_t)v >= p) p *= 10;
    return acc * p + (seq_t)v;
}

static seq_t A308006_f(seq_t x)
{
    unsigned odd = 0, digits = 0;
    if (x == 0) digits = 1;
    else {
        while (x > 0) {
            unsigned d = (unsigned)(x % 10);
            if (d & 1u) ++odd;
            ++digits;
            x /= 10;
        }
    }
    unsigned even = digits - odd;
    seq_t r = (seq_t)odd;
    r = A308006_shift(r, digits);
    r = A308006_shift(r, even);
    return r;
}

seq_t a(idx_t n)
{
    const unsigned CAP = 4096;
    seq_t *seen = mem_new(seq_t, CAP);
    unsigned cnt = 0;
    seq_t x = (seq_t)n;
    unsigned result = 0;
    for (;;) {
        unsigned found = CAP;
        for (unsigned i = 0; i < cnt; i++) {
            if (seen[i] == x) { found = i; break; }
        }
        if (found != CAP) {
            result = cnt - 1;   /* cnt is the index of the first repeated term */
            break;
        }
        if (cnt >= CAP) { result = cnt; break; }
        seen[cnt++] = x;
        x = A308006_f(x);
    }
    mem_free(seen);
    return (seq_t)result;
}
