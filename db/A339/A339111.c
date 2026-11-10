/*
 * A339111  Seeds not producing -1 under the A339091 protocol. The complement
 *          of A339093.
 * https://oeis.org/A339111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   search -- a(n) is the n-th seed s >= 1 that reappears in the
 *            A339091 sequence started from s: t(1) = s, and for i >= 2, t(i)
 *            is the running sum of the odd digits of t(1..i-1) if i is even,
 *            or the running sum of the even digits of t(1..i-1) if i is odd.
 *            A seed that never reappears is "sterile" (A3...
 * Range:     u8 n <= 70, u16 n <= 2267, u32 n <= 2267, u64 n <= 2267,
 *            u128 n <= 2267, big any n
 * Validated: standard, pass, 87 terms through n = 2168 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void A339111_add_digits(uint64_t x, uint64_t *so, uint64_t *se)
{
    while (x > 0) {
        uint64_t d = x % 10;
        if (d & 1) *so += d; else *se += d;
        x /= 10;
    }
}

static bool A339111_fertile(uint64_t s)
{
    uint64_t so = 0, se = 0;
    A339111_add_digits(s, &so, &se);
    for (uint64_t i = 2; i < 10000000ULL; ++i) {
        uint64_t cur = (i & 1) ? se : so;
        if (cur == s) return true;
        A339111_add_digits(cur, &so, &se);
        if (so > s && se > s) return false;
    }
    return false;
}

static uint64_t *A339111_list = NULL;
static size_t A339111_cnt = 0, A339111_cap = 0;
static uint64_t A339111_next = 1;

seq_t a(idx_t n)
{
    while ((idx_t)A339111_cnt < n) {
        uint64_t s = A339111_next++;
        if (A339111_fertile(s)) {
            if (A339111_cnt == A339111_cap) {
                size_t nc = A339111_cap ? A339111_cap * 2 : 256;
                uint64_t *p = (uint64_t *)realloc(A339111_list, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                A339111_list = p;
                A339111_cap = nc;
            }
            A339111_list[A339111_cnt++] = s;
        }
    }
    return (seq_t)A339111_list[n - 1];
}
