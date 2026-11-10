/*
 * A334869  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j) for all i, j >= 1, where f(1) = 0 and for n > 1,
 *          f(n) = A334870(n).
 * https://oeis.org/A334869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values among f(1..n) up to the first
 *            occurrence of f(n) (restricted growth labelling by first
 *            appearance), with f(1)=0, f(n)=A334870(n); computed by direct
 *            enumeration with hashing of a canonical form of f
 * Range:     u8 n <= 270, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9460 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A334869_isprime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A334869_nextprime(uint64_t p)
{
    p++;
    while (!A334869_isprime(p)) p++;
    return p;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N <= 1) return (seq_t)1;

    uint64_t cap = 16;
    while (cap < 2 * N) cap <<= 1;
    uint64_t *hm = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *hp = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *hid = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t next_id = 0, result = 1;

    for (uint64_t k = 1; k <= N; k++) {
        uint64_t m, p;
        if (k == 1) {
            m = 0; p = 0;
        } else {
            uint64_t t = k, best = 0;
            for (uint64_t d = 2; d * d <= t; d++) {
                if (t % d == 0) {
                    int e = 0;
                    while (t % d == 0) { t /= d; e++; }
                    if ((e & 1) && best == 0) best = d;
                }
            }
            if (t > 1 && best == 0) best = t;
            if (best == 0) {
                uint64_t r = 1;
                while (r * r < k) r++;
                m = r; p = 2;
            } else {
                m = k / best; p = best;
            }
            while (m % p == 0) {
                m /= p;
                p = A334869_nextprime(p);
            }
        }
        uint64_t h = (m * 0x9E3779B97F4A7C15ULL) ^ (p * 0xC2B2AE3D27D4EB4FULL);
        h ^= h >> 29;
        h &= (cap - 1);
        uint64_t id;
        while (1) {
            if (hid[h] == 0) {
                hm[h] = m; hp[h] = p;
                id = ++next_id;
                hid[h] = id;
                break;
            }
            if (hm[h] == m && hp[h] == p) { id = hid[h]; break; }
            h = (h + 1) & (cap - 1);
        }
        if (k == N) result = id;
    }
    free(hm); free(hp); free(hid);
    return (seq_t)result;
}
