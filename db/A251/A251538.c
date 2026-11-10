/*
 * A251538  Numbers k such that A098548(2*k+3) > A098548(2*k+1) + 6.
 * https://oeis.org/A251538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A098548(2k+3) > A098548(2k+1) + 6;
 *            A098548 computed by its search recurrence (a(1..3)=1,2,3)
 * Range:     u8 n <= 8, u16 n <= 2861, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 74 terms through n = 8810 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t s_p2, s_p1;
static idx_t s_m = 0;
static idx_t s_k = 1;
static idx_t *s_hits = 0;
static size_t s_hn = 0, s_cap = 0;

static void s_step(void)
{
    seq_t c = s_p1 + (seq_t)1;
    for (;;) {
        if (gcd(c, s_p2) != (seq_t)1 && gcd(c, s_p1) == (seq_t)1)
            break;
        ++c;
    }
    s_p2 = s_p1;
    s_p1 = c;
    s_m++;
}

seq_t a(idx_t n)
{
    if (s_m == 0) {
        s_p2 = (seq_t)2;
        s_p1 = (seq_t)3;
        s_m = 3;
    }
    while (s_hn < (size_t)n) {
        seq_t x = s_p1;          /* A098548(2k+1) */
        s_step();
        s_step();
        seq_t y = s_p1;          /* A098548(2k+3) */
        if (y > x + (seq_t)6) {
            if (s_hn == s_cap) {
                size_t nc = s_cap ? s_cap * 2 : 1024;
                s_hits = (idx_t *)realloc(s_hits, nc * sizeof(idx_t));
                s_cap = nc;
            }
            s_hits[s_hn++] = s_k;
        }
        s_k++;
    }
    return (seq_t)s_hits[n - 1];
}
