/*
 * A346593  Numbers k such that A006577(k^3) sets a new record.
 * https://oeis.org/A346593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: for k = 1, 2, 3, ... compute A006577(k^3) (odd k
 *            directly; even k = 2m can only be a record when m is a record,
 *            and then A006577((2m)^3) = A006577(m^3) + 3) and list each k
 *            that sets a new record
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A346593_u128;

#define A346593_TB 18
#define A346593_SB 20

static uint8_t  A346593_tc[1u << A346593_TB];
static uint32_t A346593_td[1u << A346593_TB];
static uint16_t A346593_small[1u << A346593_SB];
static uint64_t A346593_p3[A346593_TB + 1];
static bool     A346593_tinit = false;

static void A346593_init_tables(void)
{
    uint32_t l;
    int i;
    A346593_p3[0] = 1;
    for (i = 1; i <= A346593_TB; i++) A346593_p3[i] = A346593_p3[i - 1] * 3;
    for (l = 0; l < (1u << A346593_TB); l++) {
        uint64_t x = l;
        unsigned c = 0;
        for (i = 0; i < A346593_TB; i++) {
            if (x & 1) { x = (3 * x + 1) >> 1; c++; }
            else x >>= 1;
        }
        A346593_tc[l] = (uint8_t)c;
        A346593_td[l] = (uint32_t)x;
    }
    A346593_small[0] = 0;
    A346593_small[1] = 0;
    for (l = 2; l < (1u << A346593_SB); l++) {
        if ((l & 1) == 0) {
            A346593_small[l] = (uint16_t)(A346593_small[l >> 1] + 1);
        } else {
            uint64_t y = l;
            unsigned cnt = 0;
            while (y >= l) {
                if (y & 1) y = 3 * y + 1; else y >>= 1;
                cnt++;
            }
            A346593_small[l] = (uint16_t)(cnt + A346593_small[y]);
        }
    }
    A346593_tinit = true;
}

/* Collatz step count for a value too large for the fast 128-bit path */
static unsigned long A346593_big_steps(A346593_u128 x0)
{
    uint32_t L[256];
    int len = 0, i;
    unsigned long st = 0;
    while (x0) { L[len++] = (uint32_t)x0; x0 >>= 32; }
    for (;;) {
        if (len == 1 && L[0] == 1) break;
        if ((L[0] & 1) == 0) {
            for (i = 0; i < len - 1; i++)
                L[i] = (L[i] >> 1) | (L[i + 1] << 31);
            L[len - 1] >>= 1;
            if (L[len - 1] == 0) len--;
        } else {
            uint64_t carry = 1;
            for (i = 0; i < len; i++) {
                uint64_t v = (uint64_t)L[i] * 3 + carry;
                L[i] = (uint32_t)v;
                carry = v >> 32;
            }
            if (carry && len < 256) L[len++] = (uint32_t)carry;
        }
        st++;
    }
    return st;
}

static unsigned long A346593_steps(A346593_u128 x)
{
    unsigned long st = 0;
    const A346593_u128 LIM = (A346593_u128)1 << 98;
    const uint32_t MASK = (1u << A346593_TB) - 1;
    while (x >= ((A346593_u128)1 << A346593_SB)) {
        uint32_t l;
        if (x >= LIM) return st + A346593_big_steps(x);
        l = (uint32_t)x & MASK;
        x = (x >> A346593_TB) * A346593_p3[A346593_tc[l]] + A346593_td[l];
        st += A346593_TB + A346593_tc[l];
    }
    return st + A346593_small[(uint32_t)x];
}

seq_t a(idx_t n)
{
    /* rec[0] = 1 is an internal seed (vacuous record); rec[i], i>=1, = a(i) */
    static idx_t *rec = NULL;
    static unsigned long *rs = NULL;
    static size_t rcnt = 0, rcap = 0;
    static size_t p = 0;
    static idx_t next_k = 2;
    static unsigned long best = 0;

    if (!A346593_tinit) A346593_init_tables();
    if (rcnt == 0) {
        rcap = 64;
        rec = (idx_t *)malloc(rcap * sizeof(idx_t));
        rs = (unsigned long *)malloc(rcap * sizeof(unsigned long));
        rec[0] = 1; rs[0] = 0; rcnt = 1;
    }

    while (rcnt <= n) {
        idx_t k = next_k++;
        unsigned long s;
        if (k & 1) {
            A346593_u128 c = (A346593_u128)k * k * k;
            s = A346593_steps(c);
        } else {
            idx_t m = k >> 1;
            while (p < rcnt && rec[p] < m) p++;
            if (p >= rcnt || rec[p] != m) continue;
            s = rs[p] + 3;
        }
        if (s > best) {
            best = s;
            if (rcnt == rcap) {
                rcap *= 2;
                rec = (idx_t *)realloc(rec, rcap * sizeof(idx_t));
                rs = (unsigned long *)realloc(rs, rcap * sizeof(unsigned long));
            }
            rec[rcnt] = k;
            rs[rcnt] = s;
            rcnt++;
        }
    }
    return (seq_t)rec[n];
}
