/*
 * A387242  a(n) is the least k such that A334676(k) != k and the decimal
 *          string of n appears in A334676(k).
 * https://oeis.org/A387242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that A334676(k) != k and the decimal string
 *            of n is a substring of A334676(k); computed by search upward in
 *            k, with A334676(k) = min(k, min over digits d>1 of k with d | k
 *            of A334676(k/d)) tabulated incrementally
 * Range:     u8 n <= 14, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *a334676_tab = 0;
static uint64_t a334676_cnt = 0;   /* values for 1..cnt computed */
static uint64_t a334676_cap = 0;

static void a387242_extend(void)
{
    if (a334676_cnt + 1 >= a334676_cap) {
        uint64_t nc = a334676_cap ? a334676_cap * 2 : 1024;
        uint64_t *p = (uint64_t *)realloc(a334676_tab, (size_t)(nc + 1) * sizeof(uint64_t));
        if (!p) abort();
        a334676_tab = p;
        a334676_cap = nc;
    }
    uint64_t k = a334676_cnt + 1;
    uint64_t best = k;
    bool seen[10] = {false};
    uint64_t t = k;
    while (t > 0) {
        unsigned d = (unsigned)(t % 10);
        t /= 10;
        if (d > 1 && !seen[d]) {
            seen[d] = true;
            if (k % d == 0) {
                uint64_t c = a334676_tab[k / d];
                if (c < best) best = c;
            }
        }
    }
    a334676_tab[k] = best;
    a334676_cnt = k;
}

static int a387242_digits(uint64_t x, char *buf)
{
    char tmp[24];
    int l = 0, i;
    if (x == 0) tmp[l++] = '0';
    while (x > 0) { tmp[l++] = (char)('0' + x % 10); x /= 10; }
    for (i = 0; i < l; i++) buf[i] = tmp[l - 1 - i];
    return l;
}

seq_t a(idx_t n)
{
    char ns[24], vs[24];
    int nl = a387242_digits((uint64_t)n, ns);
    uint64_t k;
    for (k = 1;; k++) {
        while (a334676_cnt < k) a387242_extend();
        uint64_t v = a334676_tab[k];
        if (v == k) continue;
        int vl = a387242_digits(v, vs);
        int i, j;
        for (i = 0; i + nl <= vl; i++) {
            for (j = 0; j < nl; j++)
                if (vs[i + j] != ns[j]) break;
            if (j == nl) return (seq_t)k;
        }
    }
}
