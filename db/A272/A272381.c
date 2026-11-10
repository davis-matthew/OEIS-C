/*
 * A272381  Primes p == 1 (mod 3) for which A261029(10*p) = 2.
 * https://oeis.org/A272381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th prime p == 1 (mod 3) with A261029(10p) = 2; computed
 *            by a bounded search (sequence is finite), A261029 via F = s*q
 *            with s=3x+2a+b, q=a^2+ab+b^2
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static uint64_t A272381_isq(uint64_t v)
{
    uint64_t r = (uint64_t)sqrt((double)v);
    while (r * r > v) r--;
    while ((r + 1) * (r + 1) <= v) r++;
    return r;
}

static int A272381_proc(uint64_t s, uint64_t q)
{
    int c = 0;
    for (uint64_t a = 0; 3 * a * a <= 4 * q; a++) {
        uint64_t disc = 4 * q - 3 * a * a;
        uint64_t r = A272381_isq(disc);
        if (r * r != disc) continue;
        if (r < a || ((r - a) & 1)) continue;
        uint64_t b = (r - a) / 2;
        if (a + b < 1) continue;
        uint64_t sub = 2 * a + b;
        if (s >= sub && (s - sub) % 3 == 0) c++;
    }
    return c;
}

static int A272381_A261029(uint64_t n)
{
    int c = 0;
    for (uint64_t d = 1; d * d <= n; d++) {
        if (n % d) continue;
        c += A272381_proc(d, n / d);
        if (d * d != n) c += A272381_proc(n / d, d);
    }
    return c;
}

#define A272381_LIMIT 60000
static uint64_t A272381_terms[64];
static int A272381_cnt = 0;
static bool A272381_done = false;

static void A272381_build(void)
{
    static unsigned char comp[A272381_LIMIT + 1];
    for (uint64_t i = 2; i * i <= A272381_LIMIT; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= A272381_LIMIT; j += i) comp[j] = 1;
    for (uint64_t p = 7; p <= A272381_LIMIT; p++) {
        if (comp[p] || p % 3 != 1) continue;
        if (A272381_A261029(10 * p) == 2 && A272381_cnt < 64)
            A272381_terms[A272381_cnt++] = p;
    }
    A272381_done = true;
}

seq_t a(idx_t n)
{
    if (!A272381_done) A272381_build();
    if (n < 1 || n > (idx_t)A272381_cnt) return 0;
    return (seq_t)A272381_terms[n - 1];
}
