/*
 * A390097  Numbers k such that A000005(k) = A384237(k).
 * https://oeis.org/A390097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A000005(k) = A384237(k), where A384237(k) =
 *            #{d | k : d^d == d (mod k)}; found by incremental search with
 *            cached terms
 * Range:     u8 n <= 57, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t oeis390097_pm(uint64_t b, uint64_t e, uint64_t m)
{
    unsigned __int128 r = 1 % m, x = b % m;
    while (e) {
        if (e & 1) r = (r * x) % m;
        x = (x * x) % m;
        e >>= 1;
    }
    return (uint64_t)r;
}

static int oeis390097_good(uint64_t k)
{
    uint64_t tau = 0, cnt = 0, i;
    for (i = 1; i <= k / i; i++) {
        if (k % i == 0) {
            uint64_t j = k / i;
            tau++;
            if (oeis390097_pm(i, i, k) == i % k) cnt++;
            if (j != i) {
                tau++;
                if (oeis390097_pm(j, j, k) == j % k) cnt++;
            }
        }
    }
    return tau == cnt;
}

static uint64_t *oeis390097_t = NULL;
static size_t oeis390097_n = 0, oeis390097_cap = 0;
static uint64_t oeis390097_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (oeis390097_n < (size_t)n) {
        if (oeis390097_good(oeis390097_next)) {
            if (oeis390097_n == oeis390097_cap) {
                size_t nc = oeis390097_cap ? oeis390097_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(oeis390097_t, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                oeis390097_t = p;
                oeis390097_cap = nc;
            }
            oeis390097_t[oeis390097_n++] = oeis390097_next;
        }
        oeis390097_next++;
    }
    return (seq_t)oeis390097_t[n - 1];
}
