/*
 * A353705  The smallest unused number when A121216(n) is being calculated.
 * https://oeis.org/A353705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer not among A121216(1..n-1),
 *            where A121216(1)=1, A121216(2)=2, A121216(n) = least unused
 *            positive integer coprime to A121216(n-2); computed by simulation
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9869 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *s_val = NULL;   /* A121216 terms */
static uint64_t *s_min = NULL;   /* answers */
static size_t s_len = 0, s_cap = 0;
static unsigned char *s_used = NULL;
static size_t s_ucap = 0;
static uint64_t s_ptr = 1;       /* smallest unused */

static uint64_t gcd_nat(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void ensure_used(size_t idx)
{
    if (idx < s_ucap) return;
    size_t nc = s_ucap ? s_ucap : 64;
    while (nc <= idx) nc *= 2;
    s_used = (unsigned char *)realloc(s_used, nc);
    memset(s_used + s_ucap, 0, nc - s_ucap);
    s_ucap = nc;
}

static void push(uint64_t v, uint64_t m)
{
    if (s_len == s_cap) {
        s_cap = s_cap ? s_cap * 2 : 64;
        s_val = (uint64_t *)realloc(s_val, s_cap * sizeof(uint64_t));
        s_min = (uint64_t *)realloc(s_min, s_cap * sizeof(uint64_t));
    }
    s_val[s_len] = v;
    s_min[s_len] = m;
    s_len++;
    ensure_used((size_t)v);
    s_used[v] = 1;
    ensure_used((size_t)s_ptr);
    while (s_used[s_ptr]) {
        s_ptr++;
        ensure_used((size_t)s_ptr);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (s_len < (size_t)n) {
        if (s_len == 0) {
            push(1, 1);
        } else if (s_len == 1) {
            push(2, 2);
        } else {
            uint64_t m = s_ptr;
            uint64_t pp = s_val[s_len - 2];
            uint64_t c = m;
            for (;;) {
                ensure_used((size_t)c);
                if (!s_used[c] && gcd_nat(c, pp) == 1) break;
                c++;
            }
            push(c, m);
        }
    }
    return (seq_t)s_min[(size_t)(n - 1)];
}
