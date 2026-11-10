/*
 * A293894  Numbers n such that A083722(n) > 1 and A083722(n) occurs earlier
 *          in A083722.
 * https://oeis.org/A293894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k that is composite and has an earlier m<k
 *            with the same largest prime factor and no prime in [m,k], so
 *            that A083722(m) = A083722(k) > 1. Found by incremental search,
 *            scanning the composite runs between consecutive primes
 * Range:     u8 n <= 3, u16 n <= 33, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: standard, pass, 49 terms through n = 91 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static idx_t *A293894_terms = NULL;
static size_t A293894_cnt = 0, A293894_cap = 0;
static idx_t A293894_next = 2;
static idx_t *A293894_run = NULL;
static size_t A293894_runlen = 0, A293894_runcap = 0;

static idx_t A293894_lpf(idx_t x)
{
    idx_t best = 1;
    if (x % 2 == 0) {
        best = 2;
        while (x % 2 == 0) x /= 2;
    }
    for (idx_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            best = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) best = x;
    return best;
}

static void A293894_push(idx_t **arr, size_t *len, size_t *cap, idx_t v)
{
    if (*len == *cap) {
        size_t nc = *cap ? *cap * 2 : 64;
        idx_t *p = (idx_t *)realloc(*arr, nc * sizeof(idx_t));
        if (!p) exit(1);
        *arr = p;
        *cap = nc;
    }
    (*arr)[(*len)++] = v;
}

seq_t a(idx_t n)
{
    while (A293894_cnt < n) {
        idx_t x = A293894_next++;
        idx_t l = A293894_lpf(x);
        if (l == x) {
            A293894_runlen = 0;
        } else {
            int found = 0;
            for (size_t i = 0; i < A293894_runlen; ++i) {
                if (A293894_run[i] == l) { found = 1; break; }
            }
            if (found)
                A293894_push(&A293894_terms, &A293894_cnt, &A293894_cap, x);
            else
                A293894_push(&A293894_run, &A293894_runlen, &A293894_runcap, l);
        }
    }
    return (seq_t)A293894_terms[n - 1];
}
