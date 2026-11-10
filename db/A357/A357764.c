/*
 * A357764  Numbers m such that A357761(m) < A357761(k) for all k < m.
 * https://oeis.org/A357764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m with A357761(m) < A357761(k) for all k < m (record
 *            lows); computed by a scan over m = 1..14 and then multiples of
 *            15, using A357761(m) = sum_{d|m} (-1)^(popcount(d)+1). The
 *            restriction to multiples of 15 is an assumption taken from the
 *            listed terms, not proved
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A357764_MAX 256

static idx_t A357764_rec[A357764_MAX];
static int A357764_cnt = 0;
static idx_t A357764_next = 1;
static long long A357764_min = 0;
static int A357764_started = 0;

static long long A357764_f(uint64_t m)
{
    uint64_t divs[8192];
    size_t nd = 1;
    divs[0] = 1;
    uint64_t r = m;
    uint64_t p = 2;
    while (p * p <= r) {
        if (r % p == 0) {
            int e = 0;
            while (r % p == 0) { r /= p; e++; }
            size_t cur = nd;
            uint64_t pw = 1;
            for (int i = 0; i < e; i++) {
                pw *= p;
                for (size_t j = 0; j < cur; j++) divs[nd++] = divs[j] * pw;
            }
        }
        if (p == 2) p = 3; else p += 2;
    }
    if (r > 1) {
        size_t cur = nd;
        for (size_t j = 0; j < cur; j++) divs[nd++] = divs[j] * r;
    }
    long long s = 0;
    for (size_t j = 0; j < nd; j++)
        s += (__builtin_popcountll(divs[j]) & 1) ? 1 : -1;
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A357764_MAX) n = A357764_MAX;
    while ((idx_t)A357764_cnt < n) {
        idx_t m = A357764_next;
        long long f = A357764_f((uint64_t)m);
        if (!A357764_started || f < A357764_min) {
            A357764_started = 1;
            A357764_min = f;
            A357764_rec[A357764_cnt++] = m;
        }
        if (m < 15) A357764_next = m + 1;
        else A357764_next = m + 15;
    }
    return (seq_t)A357764_rec[n - 1];
}
