/*
 * A386501  Numbers k divisible by A004719(k), excluding trivial cases.
 * https://oeis.org/A386501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search over k >= 1. Write k = m*10^j with m not divisible by 10.
 *            k is a term iff m contains a 0 digit, A004719(k) | k, and
 *            A004719(k) does not divide k/10 when j > 0
 * Range:     u8 n <= 2, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 943 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint16_t A386501_bval[10000];
static uint8_t  A386501_blen[10000];
static uint64_t A386501_p10[20];
static bool A386501_init_done = false;

static void A386501_init(void) {
    int b, i;
    A386501_p10[0] = 1;
    for (i = 1; i < 20; i++) A386501_p10[i] = A386501_p10[i - 1] * 10;
    for (b = 0; b < 10000; b++) {
        int d[4], v = 0, l = 0, x = b;
        for (i = 3; i >= 0; i--) { d[i] = x % 10; x /= 10; }
        for (i = 0; i < 4; i++) {
            if (d[i] != 0) { v = v * 10 + d[i]; l++; }
        }
        A386501_bval[b] = (uint16_t)v;
        A386501_blen[b] = (uint8_t)l;
    }
    A386501_init_done = true;
}

/* Delete all zero digits of x (A004719 on native integers). */
static uint64_t A386501_del0(uint64_t x) {
    uint32_t blocks[6];
    int nb = 0, i;
    uint64_t r = 0;
    while (x) { blocks[nb++] = (uint32_t)(x % 10000); x /= 10000; }
    for (i = nb - 1; i >= 0; i--)
        r = r * A386501_p10[A386501_blen[blocks[i]]] + A386501_bval[blocks[i]];
    return r;
}

static bool A386501_test(uint64_t k) {
    uint64_t m = k, A;
    while (m % 10 == 0) m /= 10;
    A = A386501_del0(m);
    if (A == m) return false;                 /* no zero digits in m: trivial */
    if (k % A != 0) return false;
    if (k != m && (k / 10) % A == 0) return false; /* k/10 already a term */
    return true;
}

static uint64_t *A386501_terms = 0;
static idx_t A386501_cnt = 0, A386501_cap = 0;
static uint64_t A386501_next = 1;

seq_t a(idx_t n) {
    if (!A386501_init_done) A386501_init();
    while (A386501_cnt < n) {
        while (!A386501_test(A386501_next)) A386501_next++;
        if (A386501_cnt >= A386501_cap) {
            idx_t nc = A386501_cap ? A386501_cap * 2 : 1024;
            uint64_t *t = (uint64_t *)realloc(A386501_terms, (size_t)nc * sizeof(uint64_t));
            if (!t) return 0;
            A386501_terms = t;
            A386501_cap = nc;
        }
        A386501_terms[A386501_cnt++] = A386501_next;
        A386501_next++;
    }
    return (seq_t)A386501_terms[n - 1];
}
