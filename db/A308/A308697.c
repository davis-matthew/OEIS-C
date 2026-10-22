/*
 * A308697  a(n) = Sum_{d|n} d^(3*d).
 * https://oeis.org/A308697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(3*d)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Smallest prime factor table */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

static void build_spf_upto(idx_t limit)
{
    if (limit <= spf_limit) return;
    free(spf);
    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            for (idx_t j = i; j <= limit; j += i) {
                if (!spf[j]) spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

static void get_divisors(idx_t n, idx_t **divs, size_t *cnt)
{
    static idx_t *buf = NULL;
    static size_t buf_cap = 0;
    if (buf_cap == 0) {
        buf_cap = 64;
        buf = (idx_t *)malloc(buf_cap * sizeof(idx_t));
        if (!buf) exit(EXIT_FAILURE);
    }
    buf[0] = 1;
    size_t idx = 1;
    idx_t x = n;
    while (x > 1) {
        idx_t p = spf[x];
        size_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        size_t old = idx;
        for (size_t i = 0; i < old; ++i) {
            idx_t base = buf[i];
            for (size_t k = 1; k <= e; ++k) {
                base *= p;
                if (idx >= buf_cap) {
                    buf_cap *= 2;
                    buf = (idx_t *)realloc(buf, buf_cap * sizeof(idx_t));
                    if (!buf) exit(EXIT_FAILURE);
                }
                buf[idx++] = base;
            }
        }
    }
    *divs = buf;
    *cnt = idx;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    build_spf_upto(n);
    idx_t *divs;
    size_t dcnt;
    get_divisors(n, &divs, &dcnt);
    seq_t sum = 0;
    for (size_t j = 0; j < dcnt; ++j) {
        seq_t d = (seq_t)divs[j];
        seq_t exp = d * (seq_t)3;
        sum += big_pow(d, exp);
    }
    return sum;
}
