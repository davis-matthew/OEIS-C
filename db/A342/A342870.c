/*
 * A342870  a(n) is the number of twin primes between A001359(n)^2 and
 *          A001359(n)*(A001359(n)+1).
 * https://oeis.org/A342870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k in [p^2, p*(p+1)] with k and k+2 both prime,
 *            where p = A001359(n) (segmented sieve)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A001359_tw = NULL;
static size_t A001359_cnt = 0;
static uint64_t A001359_lim = 0;

static void A001359_extend(uint64_t newlim) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(newlim + 3));
    uint64_t i, j, top = newlim + 2;
    size_t cap = 1024, c = 0;
    uint64_t *list;
    memset(comp, 0, (size_t)(newlim + 3));
    comp[0] = comp[1] = 1;
    for (i = 2; i <= top / i; i++)
        if (!comp[i])
            for (j = i * i; j <= top; j += i) comp[j] = 1;
    list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 2; i <= newlim; i++) {
        if (!comp[i] && !comp[i + 2]) {
            if (c == cap) {
                cap *= 2;
                list = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
            }
            list[c++] = i;
        }
    }
    free(comp);
    free(A001359_tw);
    A001359_tw = list;
    A001359_cnt = c;
    A001359_lim = newlim;
}

static uint64_t A001359_a(uint64_t n) {
    while ((uint64_t)A001359_cnt < n) {
        uint64_t nl = A001359_lim ? A001359_lim * 2 : 4096;
        A001359_extend(nl);
    }
    return A001359_tw[n - 1];
}

seq_t a(idx_t n) {
    uint64_t p = A001359_a((uint64_t)n);
    uint64_t lo = p * p, hi = p * (p + 1);
    uint64_t top = hi + 2;
    uint64_t len = top - lo + 1;
    uint64_t r = 1;
    while (r * r <= top) r++;
    r--;
    /* small primes up to r */
    unsigned char *sm = (unsigned char *)calloc((size_t)(r + 1), 1);
    unsigned char *seg = (unsigned char *)calloc((size_t)len, 1);
    uint64_t i, j, cnt = 0;
    for (i = 2; i <= r; i++) {
        if (sm[i]) continue;
        for (j = i * i; j <= r; j += i) sm[j] = 1;
        uint64_t s = ((lo + i - 1) / i) * i;
        if (s < i * i) s = i * i;
        for (j = s; j <= top; j += i) seg[j - lo] = 1;
    }
    for (i = lo; i <= hi; i++) {
        if (i >= 2 && !seg[i - lo] && !seg[i + 2 - lo]) cnt++;
    }
    free(sm);
    free(seg);
    return (seq_t)cnt;
}
