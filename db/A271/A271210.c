/*
 * A271210  Artiads (A001583) congruent to 1 mod 50 and for which 5 is a
 *          quintic residue.
 * https://oeis.org/A271210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- sieve primes p == 1 (mod 50); keep p with
 *            Fibonacci((p-1)/5) == 0 (mod p) and 5^((p-1)/5) == 1 (mod p)
 * Range:     u8 none, u16 n <= 11, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 896 (2026-11-07)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mulmod.h"

static uint64_t A271210_fib(uint64_t k, uint64_t m) {
    uint64_t f = 0, g = 1;
    int bit = 63;
    while (bit >= 0 && !((k >> bit) & 1)) bit--;
    for (; bit >= 0; bit--) {
        uint64_t t = (2 * g % m + m - f) % m;
        uint64_t c = mulmod(f, t, m);
        uint64_t d = (mulmod(f, f, m) + mulmod(g, g, m)) % m;
        if ((k >> bit) & 1) {
            f = d;
            g = (c + d) % m;
        } else {
            f = c;
            g = d;
        }
    }
    return f;
}

static uint64_t A271210_pow5(uint64_t e, uint64_t m) {
    uint64_t r = 1, b = 5 % m;
    while (e) {
        if (e & 1) r = mulmod(r, b, m);
        b = mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static uint64_t *A271210_terms = NULL;
static idx_t A271210_cnt = 0;
static uint64_t A271210_limit = 0;

static void A271210_extend(uint64_t limit) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
    uint64_t i, j, cap;
    idx_t cnt = 0;
    uint64_t *t;
    memset(comp, 0, (size_t)(limit + 1));
    for (i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (j = i * i; j <= limit; j += i) comp[j] = 1;
    cap = 16;
    t = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    for (i = 51; i <= limit; i += 50) {
        if (comp[i]) continue;
        if (A271210_pow5((i - 1) / 5, i) != 1) continue;
        if (A271210_fib((i - 1) / 5, i) == 0) {
            if ((uint64_t)cnt == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, (size_t)cap * sizeof(uint64_t));
            }
            t[cnt++] = i;
        }
    }
    free(comp);
    free(A271210_terms);
    A271210_terms = t;
    A271210_cnt = cnt;
    A271210_limit = limit;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (A271210_cnt < n) {
        uint64_t lim = A271210_limit ? A271210_limit * 2 : 1000000;
        A271210_extend(lim);
    }
    return (seq_t)A271210_terms[n - 1];
}
