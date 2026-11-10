/*
 * A365384  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A351251(i) = A351251(j) for all i, j >= 0, where A351251(n) is
 *          the denominator of n / A276086(n).
 * https://oeis.org/A365384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of A351251(n) =
 *            A276086(n)/gcd(n, A276086(n)); computed by a memoized scan with
 *            hashing, numbering distinct values from 1 in order of first
 *            appearance
 * Range:     u8 n <= 376, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 108 terms through n = 9014 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned __int128 a365384_u128;

static a365384_u128 a365384_gcd(a365384_u128 x, a365384_u128 y)
{
    while (y) { a365384_u128 t = x % y; x = y; y = t; }
    return x;
}

static a365384_u128 a365384_key(uint64_t n)
{
    a365384_u128 v = 1;
    uint64_t m = n;
    uint64_t p = 2;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        for (uint64_t i = 0; i < d; i++) v *= p;
        /* next prime */
        uint64_t c = p + 1;
        for (;; c++) {
            bool ok = true;
            for (uint64_t q = 2; q * q <= c; q++)
                if (c % q == 0) { ok = false; break; }
            if (ok) break;
        }
        p = c;
    }
    a365384_u128 g = a365384_gcd((a365384_u128)n, v);
    return v / g;
}

static uint32_t *a365384_rgs = NULL;
static a365384_u128 *a365384_dk = NULL;
static uint32_t *a365384_tab = NULL;
static size_t a365384_cnt = 0, a365384_cap = 0, a365384_nd = 0, a365384_tsz = 0;

static size_t a365384_hash(a365384_u128 k)
{
    uint64_t h = (uint64_t)k * 0x9E3779B97F4A7C15ULL ^ (uint64_t)(k >> 64) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    return (size_t)h;
}

static void a365384_rebuild(size_t tsz)
{
    free(a365384_tab);
    a365384_tab = (uint32_t *)calloc(tsz, sizeof(uint32_t));
    a365384_tsz = tsz;
    for (size_t i = 0; i < a365384_nd; i++) {
        size_t h = a365384_hash(a365384_dk[i]) & (tsz - 1);
        while (a365384_tab[h]) h = (h + 1) & (tsz - 1);
        a365384_tab[h] = (uint32_t)(i + 1);
    }
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need > a365384_cap) {
        size_t nc = a365384_cap ? a365384_cap : 1024;
        while (nc < need) nc *= 2;
        a365384_rgs = (uint32_t *)realloc(a365384_rgs, nc * sizeof(uint32_t));
        a365384_dk = (a365384_u128 *)realloc(a365384_dk, nc * sizeof(a365384_u128));
        a365384_cap = nc;
    }
    while (a365384_cnt < need) {
        if (a365384_tsz < 2 * (a365384_nd + 2))
            a365384_rebuild(a365384_tsz ? a365384_tsz * 2 : 2048);
        a365384_u128 k = a365384_key((uint64_t)a365384_cnt);
        size_t h = a365384_hash(k) & (a365384_tsz - 1);
        uint32_t id = 0;
        while (a365384_tab[h]) {
            if (a365384_dk[a365384_tab[h] - 1] == k) { id = a365384_tab[h]; break; }
            h = (h + 1) & (a365384_tsz - 1);
        }
        if (!id) {
            a365384_dk[a365384_nd++] = k;
            id = (uint32_t)a365384_nd;
            a365384_tab[h] = id;
        }
        a365384_rgs[a365384_cnt++] = id;
    }
    return (seq_t)a365384_rgs[n];
}
