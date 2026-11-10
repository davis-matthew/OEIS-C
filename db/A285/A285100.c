/*
 * A285100  Numbers k for which A065642(k) = A285109(k).
 * https://oeis.org/A285100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A065642(k) = k*spf(k) (A285109(k)); computed
 *            by search over k, checking that no multiple of rad(k) in (k,
 *            k*spf(k)) has the same prime set as k
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9587 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A285100_ok(uint64_t k)
{
    if (k == 1) return true;
    uint64_t pr[20];
    int cnt = 0;
    uint64_t m = k, r = 1, spf = 0;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            if (!spf) spf = p;
            pr[cnt++] = p;
            r *= p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        if (!spf) spf = m;
        pr[cnt++] = m;
        r *= m;
    }
    if (cnt == 1 && k == spf) return true; /* prime: next is p^2 */
    uint64_t target = k * spf;
    for (uint64_t c = (k / r + 1) * r; c < target; c += r) {
        uint64_t t = c;
        for (int i = 0; i < cnt; ++i)
            while (t % pr[i] == 0) t /= pr[i];
        if (t == 1) return false;
    }
    return true;
}

static uint64_t *A285100_terms = NULL;
static size_t A285100_cnt = 0, A285100_cap = 0;
static uint64_t A285100_last = 0;

seq_t a(idx_t n)
{
    while (A285100_cnt < (size_t)n) {
        ++A285100_last;
        if (A285100_ok(A285100_last)) {
            if (A285100_cnt == A285100_cap) {
                A285100_cap = A285100_cap ? A285100_cap * 2 : 1024;
                A285100_terms = (uint64_t *)realloc(A285100_terms, A285100_cap * sizeof(uint64_t));
            }
            A285100_terms[A285100_cnt++] = A285100_last;
        }
    }
    return (seq_t)A285100_terms[n - 1];
}
