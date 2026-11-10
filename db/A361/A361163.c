/*
 * A361163  Number of elliptic points of order 3 in Weierstrass curve with
 *          discriminant A361160(n) in moduli space M_3 of compact Riemann
 *          surfaces of genus 3.
 * https://oeis.org/A361163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = h(-3D)/2 if 3 does not divide D, else (h(-3D) -
 *            2*h(-D/3))/2, where D = A361160(n) (fitted to the data, not a
 *            proved formula). h(Δ) is the class number of primitive binary
 *            quadratic forms of discriminant Δ. The value is 0 unless D is
 *            odd or D mod 16 is 8 or 12. Computed by counting reduced...
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A361160_a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 1;
    static seq_t sqrt_r = 0;
    static seq_t next_square = 0;

    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return 0;
    }
    while (cache_size < n) {
        bool ok = (next_candidate % 4 == 0) || (next_candidate % 8 == 1);
        if (ok) {
            while (next_square < next_candidate) {
                sqrt_r++;
                next_square = sqrt_r * sqrt_r;
            }
            if (next_square != next_candidate) {
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    if (tmp == NULL) return 0;
                    cache = tmp;
                }
                cache[cache_size++] = next_candidate;
            }
        }
        next_candidate++;
    }
    return cache[n - 1];
}

static long long A361163_gcd(long long x, long long y)
{
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (y) { long long t = x % y; x = y; y = t; }
    return x;
}

/* class number of primitive forms with discriminant -m (m > 0) */
static long long A361163_h(long long m)
{
    long long cnt = 0;
    long long a, b;
    for (a = 1; 3 * a * a <= m; a++) {
        for (b = -a + 1; b <= a; b++) {
            if (((b - m) & 1) != 0) continue; /* b ≡ m (mod 2) */
            long long num = b * b + m;
            if (num % (4 * a) != 0) continue;
            long long c = num / (4 * a);
            if (c < a) continue;
            if (c == a && b < 0) continue;
            if (A361163_gcd(A361163_gcd(a, b), c) != 1) continue;
            cnt++;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    long long D = (long long)A361160_a(n);
    if (D % 2 == 0) {
        long long r = D % 16;
        if (r != 8 && r != 12) return (seq_t)0;
    }
    long long res;
    if (D % 3 != 0) {
        res = A361163_h(3 * D) / 2;
    } else {
        res = (A361163_h(3 * D) - 2 * A361163_h(D / 3)) / 2;
    }
    if (res < 0) res = 0;
    return (seq_t)res;
}
