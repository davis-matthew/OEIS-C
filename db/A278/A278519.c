/*
 * A278519  a(n) = largest k for which A260731(k) = n.
 * https://oeis.org/A278519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k with A260731(k) = n; A260731(0)=0, A260731(k) =
 *            1 + A260731(k - A002828(k)). Computed by sieve: first k with
 *            A260731(k) > n, minus 1
 * Range:     u8 n <= 91, u16 n <= 11363, u32 n <= 11363, u64 n <= 11363,
 *            u128 n <= 11363, big any n
 * Validated: standard, pass, 90 terms through n = 9077 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *A278519_st = NULL;
static uint64_t A278519_cnt = 0, A278519_cap = 0;

static unsigned A278519_minsq(uint64_t k)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= k) r++;
    if (r * r == k) return 1;
    for (uint64_t j = 1; j * j <= k; j++) {
        uint64_t rem = k - j * j;
        uint64_t s = 0;
        while ((s + 1) * (s + 1) <= rem) s++;
        if (s * s == rem) return 2;
    }
    uint64_t m = k;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return 4;
    return 3;
}

static void A278519_push(void)
{
    if (A278519_cnt == A278519_cap) {
        uint64_t nc = A278519_cap ? A278519_cap * 2 : 1024;
        A278519_st = (uint32_t *)realloc(A278519_st, nc * sizeof(uint32_t));
        A278519_cap = nc;
    }
    uint64_t k = A278519_cnt;
    if (k == 0) A278519_st[0] = 0;
    else A278519_st[k] = A278519_st[k - A278519_minsq(k)] + 1;
    A278519_cnt++;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t k = 1;
    for (;;) {
        while (A278519_cnt <= k) A278519_push();
        if ((uint64_t)A278519_st[k] > (uint64_t)n) break;
        k++;
    }
    return (seq_t)(k - 1);
}
