/*
 * A308411  Number of ways to write n as 2^i*3^j + A008347(k), where i, j and
 *          k > 0 are nonnegative integers.
 * https://oeis.org/A308411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (i,j,k), i,j >= 0, k >= 1, with n = 2^i*3^j +
 *            A008347(k); computed by direct search over k (A008347(k) =
 *            prime(k) - A008347(k-1), A008347(0) = 0)
 * Range:     u8 n <= 924, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9479 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t A308411_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 64;
        pr = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (!pr) abort();
    }
    while (sz < (size_t)k) {
        if (sz == 0) {
            pr[sz++] = 2;
            continue;
        }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t q = pr[i];
            if ((uint64_t)q * q > (uint64_t)p) break;
            if (p % q == 0) { ok = false; break; }
        }
        if (!ok) { cand += 2; continue; }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }
    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t count = 0;
    seq_t prev = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t cur = (seq_t)A308411_get_prime(k) - prev;
        if (cur < N) {
            seq_t x = N - cur;
            while (x % 2 == 0) x /= 2;
            while (x % 3 == 0) x /= 3;
            if (x == 1) count++;
        }
        /* odd- and even-indexed subsequences are nondecreasing */
        if (k >= 2 && cur >= N && prev >= N) break;
        prev = cur;
    }
    return count;
}
