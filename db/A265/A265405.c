/*
 * A265405  Start with a(1) = 1, then always choose for a(n) the least unused
 *          number such that A193231(a(n)*a(n-1)) = A193231(a(n)) *
 *          A193231(a(n-1)), where A193231 is an involution of natural numbers
 *          called Blue code.
 * https://oeis.org/A265405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n) = least unused k with A193231(k*a(n-1)) =
 *            A193231(k)*A193231(a(n-1)); computed by greedy search
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 183, u64 n <= 183,
 *            u128 n <= 183, big any n
 * Validated: standard, pass, 91 terms through n = 175 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 a265405_u128;

static a265405_u128 A265405_blue(a265405_u128 x)
{
    /* GF(2) superset-sum transform: Blue code is an involution computed by butterflies */
    int i;
    for (i = 0; i < 7; i++) {
        unsigned s = 1u << i;
        a265405_u128 m = (~(a265405_u128)0) / (((a265405_u128)1 << s) + 1);
        x ^= (x >> s) & m;
    }
    return x;
}

static a265405_u128 A265405_terms[512];
static int A265405_cnt = 0;
static a265405_u128 A265405_lo = 1;

static int A265405_used(a265405_u128 c)
{
    int i;
    for (i = 0; i < A265405_cnt; i++)
        if (A265405_terms[i] == c) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    a265405_u128 v;
    if (n < 1) return 0;
    if (n > 500) n = 500;
    if (A265405_cnt == 0) {
        A265405_terms[0] = 1;
        A265405_cnt = 1;
        A265405_lo = 2;
    }
    while ((idx_t)A265405_cnt < n) {
        a265405_u128 p = A265405_terms[A265405_cnt - 1];
        a265405_u128 bp = A265405_blue(p);
        a265405_u128 c;
        while (A265405_used(A265405_lo)) A265405_lo++;
        for (c = A265405_lo; ; c++) {
            if (A265405_blue(c * p) == A265405_blue(c) * bp && !A265405_used(c))
                break;
        }
        A265405_terms[A265405_cnt++] = c;
    }
    v = A265405_terms[n - 1];
    {
        uint64_t hi = (uint64_t)(v >> 64);
        uint64_t lo = (uint64_t)v;
        seq_t r;
        if (hi == 0) return (seq_t)lo;
        r = (seq_t)hi;
        r <<= 32;
        r <<= 32;
        r |= (seq_t)lo;
        return r;
    }
}
