/*
 * A328577  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(0) = 0 and f(n>0) = A328572(n), for all i,
 *          j.
 * https://oeis.org/A328577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of f, f(0)=0,
 *            f(n)=A328572(n)=A003557(A276086(n)); computed by hashing f
 *            values (primorial-base digits d>0 give p^(d-1)) and numbering
 *            first occurrences 1,2,3,
 * Range:     u8 n <= 1343, u16 n <= 32768, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 122 terms through n = 9825 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

typedef unsigned __int128 A328577_u128;

static A328577_u128 A328577_f(uint64_t m)
{
    if (m == 0) return 0;
    A328577_u128 prod = 1;
    uint64_t p = 2;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        for (uint64_t e = 1; e < d; e++) prod *= (A328577_u128)p;
        /* next prime */
        uint64_t c = p + 1;
        for (;;) {
            int ok = 1;
            for (uint64_t q = 2; q * q <= c; q++)
                if (c % q == 0) { ok = 0; break; }
            if (ok) break;
            c++;
        }
        p = c;
    }
    return prod;
}

static uint64_t A328577_hash(A328577_u128 k)
{
    uint64_t x = (uint64_t)k ^ ((uint64_t)(k >> 64) * 0x9E3779B97F4A7C15ULL);
    x ^= x >> 33; x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

seq_t a(idx_t n)
{
    size_t cnt = (size_t)n + 1;
    size_t cap = 16;
    while (cap < 2 * cnt) cap <<= 1;
    A328577_u128 *keys = (A328577_u128 *)malloc(cnt * sizeof(A328577_u128));
    uint64_t *ids = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    int64_t *tab = (int64_t *)malloc(cap * sizeof(int64_t));
    if (!keys || !ids || !tab) abort();
    for (size_t i = 0; i < cap; i++) tab[i] = -1;
    uint64_t next_id = 1;
    uint64_t result = 1;
    for (size_t i = 0; i < cnt; i++) {
        A328577_u128 k = A328577_f((uint64_t)i);
        keys[i] = k;
        size_t h = (size_t)(A328577_hash(k) & (cap - 1));
        for (;;) {
            if (tab[h] < 0) {
                tab[h] = (int64_t)i;
                ids[i] = next_id++;
                break;
            }
            if (keys[tab[h]] == k) {
                ids[i] = ids[tab[h]];
                break;
            }
            h = (h + 1) & (cap - 1);
        }
        result = ids[i];
    }
    free(keys);
    free(ids);
    free(tab);
    return (seq_t)result;
}
