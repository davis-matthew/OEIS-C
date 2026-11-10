/*
 * A336561  Numbers k at which point A336459(k) appears multiplicative, but
 *          A051027(k) does not.
 * https://oeis.org/A336561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A065330(sigma(sigma(k))) == Product_{p^e ||
 *            k} A065330(sigma(sigma(p^e))) but sigma(sigma(k)) !=
 *            Product_{p^e || k} sigma(sigma(p^e)); found by incremental
 *            search over k, cached
 * Range:     u8 none, u16 n <= 140, u32 n <= 11481, u64 n <= 11481,
 *            u128 n <= 11481, big any n
 * Validated: standard, pass, 64 terms through n = 9917 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static uint64_t A336561_sigma_pp(uint64_t p, uint64_t e)
{
    uint64_t t = 1, pw = 1;
    for (uint64_t i = 0; i < e; ++i) { pw *= p; t += pw; }
    return t;
}

static uint64_t A336561_sigma(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t r = 1;
    if (n <= 1) return 1;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) r *= A336561_sigma_pp(pr[i], ex[i]);
    return r;
}

static uint64_t A336561_strip(uint64_t x)
{
    while (x && x % 2 == 0) x /= 2;
    while (x && x % 3 == 0) x /= 3;
    return x;
}

static bool A336561_test(uint64_t k)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    if (k < 2) return false;
    factor_u64(k, pr, ex, &cnt);
    if (cnt < 2) return false;
    unsigned __int128 pg = 1, pa = 1;
    uint64_t sk = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t s = A336561_sigma_pp(pr[i], ex[i]);
        sk *= s;
        uint64_t ss = A336561_sigma(s);
        pg *= ss;
        pa *= A336561_strip(ss);
    }
    uint64_t SS = A336561_sigma(sk);
    if ((unsigned __int128)A336561_strip(SS) != pa) return false;
    return (unsigned __int128)SS != pg;
}

static uint64_t *A336561_terms = NULL;
static size_t A336561_cnt = 0, A336561_cap = 0;
static uint64_t A336561_next = 2;

seq_t a(idx_t n)
{
    while (A336561_cnt < (size_t)n) {
        if (A336561_test(A336561_next)) {
            if (A336561_cnt == A336561_cap) {
                A336561_cap = A336561_cap ? A336561_cap * 2 : 1024;
                A336561_terms = (uint64_t *)realloc(A336561_terms, A336561_cap * sizeof(uint64_t));
            }
            A336561_terms[A336561_cnt++] = A336561_next;
        }
        ++A336561_next;
    }
    return (seq_t)A336561_terms[n - 1];
}
