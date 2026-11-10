/*
 * A253288  Each term a(n) satisfies four properties: 1, divisible by all
 *          prime factors of n; 2, divisible by only the prime factors of n;
 *          3, not equal to any of the terms a(1), a(2), ... a(n-1); 4,
 *          smallest number satisfying 1-3 if A005361(n) is even, or second
 *          smallest number satisfying 1-3 if A005361(n) i...
 * https://oeis.org/A253288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulated search. Within the class of numbers with the same
 *            radical as n, process the class members in increasing order.
 *            Member m takes the smallest unused class element if A005361(m)
 *            is even, or the second smallest unused if it is odd. a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9722 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct { uint64_t v; int odd; } A253288_item;

static A253288_item *A253288_buf;
static size_t A253288_cnt, A253288_cap;
static uint64_t A253288_B;
static uint64_t A253288_pr[24];
static int A253288_np;

static void A253288_push(uint64_t v, int odd) {
    if (A253288_cnt == A253288_cap) {
        A253288_cap = A253288_cap ? A253288_cap * 2 : 64;
        A253288_buf = (A253288_item *)realloc(A253288_buf, A253288_cap * sizeof(A253288_item));
    }
    A253288_buf[A253288_cnt].v = v;
    A253288_buf[A253288_cnt].odd = odd;
    A253288_cnt++;
}

static void A253288_gen(int i, uint64_t v, int allodd) {
    if (i == A253288_np) {
        A253288_push(v, allodd);
        return;
    }
    uint64_t p = A253288_pr[i];
    uint64_t cur = v;
    for (int e = 1;; e++) {
        if (cur > A253288_B / p) break;
        cur *= p;
        A253288_gen(i + 1, cur, allodd && (e & 1));
    }
}

static int A253288_cmp(const void *x, const void *y) {
    uint64_t a = ((const A253288_item *)x)->v, b = ((const A253288_item *)y)->v;
    return a < b ? -1 : (a > b ? 1 : 0);
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t m = (uint64_t)n;
    A253288_np = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            A253288_pr[A253288_np++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) A253288_pr[A253288_np++] = m;

    uint64_t B = (uint64_t)n;
    size_t k = 0;
    for (;;) {
        A253288_cnt = 0;
        A253288_B = B;
        A253288_gen(0, 1, 1);
        qsort(A253288_buf, A253288_cnt, sizeof(A253288_item), A253288_cmp);
        if (k == 0) {
            k = A253288_cnt; /* n is the last member <= n */
        }
        if (A253288_cnt >= k + 1) break;
        B *= 2;
    }

    size_t total = A253288_cnt;
    bool *used = (bool *)calloc(total, sizeof(bool));
    size_t first = 0;
    uint64_t result = 0;
    for (size_t j = 0; j < k; j++) {
        while (first < total && used[first]) first++;
        size_t pick = first;
        if (A253288_buf[j].odd == 0) {
            /* A005361 even: smallest */
        } else {
            /* odd product: second smallest unused */
            size_t q = first + 1;
            while (q < total && used[q]) q++;
            pick = q;
        }
        used[pick] = true;
        result = A253288_buf[pick].v;
    }
    free(used);
    return (seq_t)result;
}
