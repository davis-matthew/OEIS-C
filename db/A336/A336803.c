/*
 * A336803  Numbers k such that A230319(k) - A230319(k-1) = 2.
 * https://oeis.org/A336803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A230319(k) - A230319(k-1) = 2, where
 *            A230319(k) = least m>0 with m! > m^k; found by a monotone search
 *            using logarithms
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

static uint64_t *A336803_terms = NULL;
static size_t A336803_cnt = 0;
static size_t A336803_cap = 0;
static uint64_t A336803_k = 0;          /* last k processed */
static uint64_t A336803_m = 2;          /* A230319(A336803_k) */
static double A336803_lf = 0.6931471805599453; /* ln(m!) */

static void A336803_extend(void)
{
    uint64_t prev = A336803_m;
    uint64_t k = ++A336803_k;
    while (!(A336803_lf > (double)k * log((double)A336803_m))) {
        A336803_m++;
        A336803_lf += log((double)A336803_m);
    }
    if (A336803_m - prev == 2) {
        if (A336803_cnt == A336803_cap) {
            size_t nc = A336803_cap ? A336803_cap * 2 : 256;
            uint64_t *t = (uint64_t *)realloc(A336803_terms, nc * sizeof(uint64_t));
            if (!t) abort();
            A336803_terms = t;
            A336803_cap = nc;
        }
        A336803_terms[A336803_cnt++] = k;
    }
}

seq_t a(idx_t n)
{
    while (A336803_cnt < (size_t)n)
        A336803_extend();
    return (seq_t)A336803_terms[(size_t)n - 1];
}
