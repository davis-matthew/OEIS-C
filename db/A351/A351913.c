/*
 * A351913  Least k such that A352483(k) = n, or -1 if no such k exists.
 * https://oeis.org/A351913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A352483(k) = n, else -1; found by sieving
 *            d(k), searching k up to a fixed limit
 * Range:     u8 n <= 5, u16 n <= 59, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Validated: standard, pass, 75 terms through n = 95 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A351913_LIMIT 15000000u
#define A351913_MAXN 4096u

static uint64_t A351913_first[A351913_MAXN + 1];
static bool A351913_done = false;

static uint64_t A351913_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static void A351913_build(void)
{
    uint32_t N = A351913_LIMIT;
    uint32_t i, j;
    uint16_t *d = (uint16_t *)calloc((size_t)N + 1, sizeof(uint16_t));
    for (i = 0; i <= A351913_MAXN; i++) A351913_first[i] = 0;
    if (!d) { A351913_done = true; return; }
    for (i = 1; i <= N; i++)
        for (j = i; j <= N; j += i) d[j]++;
    for (uint32_t k = 2; k <= N; k++) {
        uint64_t dd = d[k];
        if (dd >= k) continue;
        uint64_t g1 = A351913_gcd(k, dd);
        uint64_t a = k / g1, b = dd / g1;
        uint64_t diff = a - b;
        uint64_t g = g1 * A351913_gcd(diff, g1);
        uint64_t val = ((uint64_t)k - dd) / g;
        if (val >= 1 && val <= A351913_MAXN && A351913_first[val] == 0)
            A351913_first[val] = k;
    }
    free(d);
    A351913_done = true;
}

seq_t a(idx_t n)
{
    if (!A351913_done) A351913_build();
    if (n >= 1 && n <= A351913_MAXN && A351913_first[n] != 0)
        return (seq_t)A351913_first[n];
    return (seq_t)-1;
}
