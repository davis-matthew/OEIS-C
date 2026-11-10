/*
 * A392755  a(n) = the largest value of k such that A381466(k) = n or -1 if no
 *          such k exists.
 * https://oeis.org/A392755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = max{k : A381466(k) = n}, or -1 if none; found by scanning
 *            A381466 (a(0)=4; a(k)=a(k-1)+k if gcd(a(k-1),k)=1 else k/gcd)
 *            for k <= 6n^2-2n
 * Range:     i8 n <= 8, i16 n <= 838, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Validated: standard, pass, 79 terms through n = 1661 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A392755_cache = NULL;
static uint64_t A392755_len = 0;
static uint64_t A392755_cap = 0;

static uint64_t A392755_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static void A392755_extend(uint64_t last)
{
    if (last + 1 <= A392755_len)
        return;
    if (last + 1 > A392755_cap) {
        uint64_t nc = A392755_cap ? A392755_cap * 2 : 1024;
        if (nc < last + 1)
            nc = last + 1;
        uint64_t *tmp = (uint64_t *)realloc(A392755_cache, nc * sizeof(uint64_t));
        if (!tmp)
            abort();
        A392755_cache = tmp;
        A392755_cap = nc;
    }
    if (A392755_len == 0) {
        A392755_cache[0] = 4;
        A392755_len = 1;
    }
    for (uint64_t i = A392755_len; i <= last; i++) {
        uint64_t prev = A392755_cache[i - 1];
        uint64_t g = A392755_gcd(prev, i);
        A392755_cache[i] = (g == 1) ? prev + i : i / g;
    }
    A392755_len = last + 1;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t bound = 6 * m * m - 2 * m;
    A392755_extend(bound);
    int found = 0;
    uint64_t best = 0;
    for (uint64_t k = bound + 1; k-- > 0;) {
        if (A392755_cache[k] == m) {
            found = 1;
            best = k;
            break;
        }
    }
    if (!found)
        return (seq_t)(-1);
    return (seq_t)best;
}
