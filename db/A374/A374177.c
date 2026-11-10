/*
 * A374177  Positions of records in A373330.
 * https://oeis.org/A374177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form: a(n) is the n-th m >= 0 at which
 *            A373330(m) = m^2*(m^2+1)/2 - floor(sqrt(m^2*(m^2+1)/2))^2
 *            reaches a new record (a(1) = 0). floor(sqrt) is updated
 *            incrementally in native 128-bit arithmetic as m grows
 * Range:     u8 n <= 55, u16 n <= 880, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8889 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 A374177_u128;

static uint64_t *A374177_rec = 0;
static size_t A374177_cnt = 0, A374177_cap = 0;
static uint64_t A374177_m = 0;          /* last m processed */
static A374177_u128 A374177_s = 0;      /* floor(sqrt(T(m))) for last m */
static A374177_u128 A374177_lastd = 0;  /* last increment of s */
static A374177_u128 A374177_best = 0;   /* current record value */
static int A374177_init = 0;

static void A374177_push(uint64_t m)
{
    if (A374177_cnt == A374177_cap) {
        A374177_cap = A374177_cap ? A374177_cap * 2 : 4096;
        A374177_rec = (uint64_t *)realloc(A374177_rec, A374177_cap * sizeof(uint64_t));
    }
    A374177_rec[A374177_cnt++] = m;
}

seq_t a(idx_t n)
{
    if (!A374177_init) {
        A374177_init = 1;
        A374177_m = 0;
        A374177_s = 0;
        A374177_lastd = 0;
        A374177_best = 0;       /* A373330(0) = 0 */
        A374177_push(0);
    }
    while (A374177_cnt < (size_t)n) {
        uint64_t m = ++A374177_m;
        A374177_u128 m2 = (A374177_u128)m * m;
        A374177_u128 T = m2 * (m2 + 1) / 2;
        A374177_u128 s_old = A374177_s;
        A374177_u128 s = s_old + A374177_lastd;   /* good guess, off by a few */
        while (s * s > T) s--;
        while ((s + 1) * (s + 1) <= T) s++;
        A374177_lastd = s - s_old;
        A374177_s = s;
        A374177_u128 v = T - s * s;
        if (v > A374177_best) {
            A374177_best = v;
            A374177_push(m);
        }
    }
    return (seq_t)A374177_rec[(size_t)n - 1];
}
