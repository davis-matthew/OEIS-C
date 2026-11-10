/*
 * A389229  Smallest k for which A388258(k) = n, or -1 if no such k exists.
 * https://oeis.org/A389229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k with A388258(k) = n, else -1; A388258(k) = #{d
 *            | k : d^d == -(-d)^k (mod k)}. Computed by incremental search
 *            over k with an spf sieve
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A389229_LIM (1u << 24)
#define A389229_CAP 4096

static uint32_t *A389229_spf = 0;
static uint32_t A389229_scanned = 0;
static uint32_t A389229_first[A389229_CAP];

static uint64_t A389229_pm(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

static bool A389229_ok(uint64_t d, uint64_t k)
{
    uint64_t left = A389229_pm(d, d, k);
    uint64_t t = A389229_pm((k - d) % k, k, k);
    uint64_t right = (k - t) % k;
    return left == right;
}

static uint32_t A389229_count(uint32_t k)
{
    uint32_t divs[2048];
    uint32_t nd = 1, m = k, c = 0;
    divs[0] = 1;
    while (m > 1) {
        uint32_t p = A389229_spf[m];
        uint32_t e = 0;
        while (m % p == 0) { m /= p; e++; }
        uint32_t cur = nd;
        uint32_t mult = 1;
        for (uint32_t j = 0; j < e; j++) {
            mult *= p;
            for (uint32_t i = 0; i < cur; i++)
                divs[nd++] = divs[i] * mult;
        }
    }
    for (uint32_t i = 0; i < nd; i++)
        if (A389229_ok(divs[i], k)) c++;
    return c;
}

seq_t a(idx_t n)
{
    if (A389229_spf == 0) {
        A389229_spf = (uint32_t *)calloc((size_t)A389229_LIM + 1, sizeof(uint32_t));
        if (!A389229_spf) return (seq_t)0 - 1;
        for (uint32_t i = 2; i <= A389229_LIM; i++) {
            if (A389229_spf[i] == 0) {
                for (uint32_t j = i; j <= A389229_LIM; j += i) {
                    if (A389229_spf[j] == 0) A389229_spf[j] = i;
                    if (j > A389229_LIM - i) break;
                }
            }
        }
    }
    if (n >= A389229_CAP) return (seq_t)0 - 1;
    while (A389229_first[n] == 0 && A389229_scanned < A389229_LIM) {
        A389229_scanned++;
        uint32_t c = A389229_count(A389229_scanned);
        if (c < A389229_CAP && A389229_first[c] == 0)
            A389229_first[c] = A389229_scanned;
    }
    if (A389229_first[n] == 0) return (seq_t)0 - 1;
    return (seq_t)A389229_first[n];
}
