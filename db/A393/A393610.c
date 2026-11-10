/*
 * A393610  a(n) = least positive integer k such that A003627(n) + 3^k - 1 is
 *          prime, or -1 if no such prime exists.
 * https://oeis.org/A393610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k>=1 such that A003627(n) + 3^k - 1 is prime, or -1
 *            if none (n=1: 3^k+1 is even and >2); found by search
 * Range:     i8 n <= 15, i16 n <= 52, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_prime_seq.h"

static uint64_t *A003627_cache = NULL;
static idx_t A003627_len = 0;
static idx_t A003627_cap = 0;

static uint64_t A003627_get(idx_t n) {
    if (A003627_len == 0) {
        A003627_cap = 64;
        A003627_cache = (uint64_t *)malloc(A003627_cap * sizeof(uint64_t));
        A003627_cache[0] = 2;
        A003627_len = 1;
    }
    while (A003627_len < n) {
        uint64_t c = A003627_cache[A003627_len - 1] + 3;
        if (A003627_len == 1) c = 5;
        while (!is_prime_u64(c)) c += 3;
        if (A003627_len == A003627_cap) {
            A003627_cap *= 2;
            A003627_cache = (uint64_t *)realloc(A003627_cache, A003627_cap * sizeof(uint64_t));
        }
        A003627_cache[A003627_len++] = c;
    }
    return A003627_cache[n - 1];
}

seq_t a(idx_t n) {
    uint64_t p = A003627_get(n);
    if (p == 2) return (seq_t)(-1);
    seq_t base = (seq_t)p - 1;
    seq_t t = 1;
    for (int k = 1; k < 100000; k++) {
        t *= 3;
        if (is_prime_seq(base + t)) return (seq_t)k;
    }
    return (seq_t)(-1);
}
