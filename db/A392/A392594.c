/*
 * A392594  Numbers k such that A003415(k) == A276085(k) (mod 4), where
 *          A003415 is the arithmetic derivative and A276085 is the primorial
 *          base log-function.
 * https://oeis.org/A392594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A003415(k) == A276085(k) (mod 4); found
 *            by search. Mod 4: A276085(k) == v2(k) + 2*(number of odd prime
 *            factors with multiplicity), A003415(k) == Sum_{p^e || k} e*(k/p)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9804 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *tt_terms = NULL;
static size_t tt_cnt = 0, tt_cap = 0;
static uint64_t tt_last = 0;

static bool tt_ok(uint64_t k)
{
    if (k == 1) return true;
    uint64_t m = k, der = 0, lg = 0;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            der += (e & 3u) * ((k / p) & 3u);
            if (p == 2) lg += e;
            else lg += 2 * e;
        }
    }
    if (m > 1) {
        der += (k / m) & 3u;
        if (m == 2) lg += 1;
        else lg += 2;
    }
    return ((der - lg) & 3u) == 0;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    while (tt_cnt < need) {
        ++tt_last;
        if (tt_ok(tt_last)) {
            if (tt_cnt == tt_cap) {
                size_t nc = tt_cap ? tt_cap * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(tt_terms, nc * sizeof(uint64_t));
                if (!t) abort();
                tt_terms = t;
                tt_cap = nc;
            }
            tt_terms[tt_cnt++] = tt_last;
        }
    }
    return (seq_t)tt_terms[need - 1];
}
