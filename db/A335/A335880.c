/*
 * A335880  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A329697(i) = A329697(j) and A331410(i) = A331410(j) for all i,
 *          j >= 1.
 * https://oeis.org/A335880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the pair [A329697(n),
 *            A331410(n)]; computed by scanning m = 1..n and numbering
 *            distinct pairs in order of first appearance
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9938 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A335880_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) { largest = 2; m >>= 1; }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return largest;
}

static uint64_t A335880_down(uint64_t k)
{
    uint64_t c = 0;
    while (k & (k - 1)) {
        uint64_t p = A335880_lpf(k);
        k -= k / p;
        c++;
    }
    return c;
}

static uint64_t A335880_up(uint64_t k)
{
    uint64_t c = 0;
    while (k & (k - 1)) {
        uint64_t p = A335880_lpf(k);
        k += k / p;
        c++;
    }
    return c;
}

static uint32_t *A335880_ids = 0;
static uint64_t A335880_cnt = 0, A335880_cap = 0;
static uint64_t *A335880_pa = 0, *A335880_pb = 0;
static uint64_t A335880_np = 0, A335880_pcap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > A335880_cap) {
        uint64_t nc = A335880_cap ? A335880_cap : 1024;
        while (nc < n) nc *= 2;
        A335880_ids = (uint32_t *)realloc(A335880_ids, nc * sizeof(uint32_t));
        A335880_cap = nc;
    }
    while (A335880_cnt < n) {
        uint64_t m = A335880_cnt + 1;
        uint64_t x = A335880_down(m);
        uint64_t y = A335880_up(m);
        uint64_t i;
        for (i = 0; i < A335880_np; i++)
            if (A335880_pa[i] == x && A335880_pb[i] == y) break;
        if (i == A335880_np) {
            if (A335880_np == A335880_pcap) {
                A335880_pcap = A335880_pcap ? A335880_pcap * 2 : 256;
                A335880_pa = (uint64_t *)realloc(A335880_pa, A335880_pcap * sizeof(uint64_t));
                A335880_pb = (uint64_t *)realloc(A335880_pb, A335880_pcap * sizeof(uint64_t));
            }
            A335880_pa[i] = x;
            A335880_pb[i] = y;
            A335880_np++;
        }
        A335880_ids[A335880_cnt] = (uint32_t)(i + 1);
        A335880_cnt++;
    }
    return (seq_t)A335880_ids[n - 1];
}
