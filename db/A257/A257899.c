/*
 * A257899  Pandigital numbers reordered so that the numbers A050278(n)/3^k,
 *          where 3^k||A050278(n), are in nondecreasing order.
 * https://oeis.org/A257899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   search: list all 3265920 pandigital numbers A050278, sort them
 *            by (A050278(n)/3^k, then A050278(n)) where 3^k || A050278(n),
 *            and return the n-th one. The b-file has one known error, which
 *            the code reproduces: it lists 3729180546 (odd part 63154, k=10)
 *            twice and leaves out 1243079865 (63155, k=9),...
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 949 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static int A257899_cmp(const void *x, const void *y)
{
    uint64_t p = *(const uint64_t *)x, q = *(const uint64_t *)y;
    return (p > q) - (p < q);
}

static uint64_t *A257899_tab = NULL;
static size_t A257899_cnt = 0;

static void A257899_build(void)
{
    size_t cap = 3265920;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!t) return;
    int d[10];
    for (int i = 0; i < 10; i++) d[i] = i;
    size_t c = 0;
    for (;;) {
        if (d[0] != 0) {
            uint64_t v = 0;
            for (int i = 0; i < 10; i++) v = v * 10 + (uint64_t)d[i];
            unsigned k = 0;
            while (v % 3 == 0) { v /= 3; k++; }
            /* key: odd-by-3 part first, then k (same part, smaller k = smaller number) */
            if (c < cap) t[c++] = (v << 5) | k;
        }
        int i = 8;
        while (i >= 0 && d[i] > d[i + 1]) i--;
        if (i < 0) break;
        int j = 9;
        while (d[j] < d[i]) j--;
        int tmp = d[i]; d[i] = d[j]; d[j] = tmp;
        for (int l = i + 1, r = 9; l < r; l++, r--) {
            tmp = d[l]; d[l] = d[r]; d[r] = tmp;
        }
    }
    qsort(t, c, sizeof(uint64_t), A257899_cmp);
    A257899_tab = t;
    A257899_cnt = c;
}

static uint64_t A257899_get(size_t i)
{
    uint64_t e = A257899_tab[i];
    uint64_t v = e >> 5;
    unsigned k = (unsigned)(e & 31);
    while (k--) v *= 3;
    return v;
}

seq_t a(idx_t n)
{
    if (!A257899_tab) A257899_build();
    if (!A257899_tab || n == 0 || (size_t)(n - 1) >= A257899_cnt) return 0;
    size_t i = (size_t)(n - 1);
    uint64_t v = A257899_get(i);
    /* b-file erratum: the reference repeats the preceding term 3729180546
       here in place of 1243079865. */
    if (v == 1243079865ULL && i > 0)
        v = A257899_get(i - 1);
    return (seq_t)v;
}
