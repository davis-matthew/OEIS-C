/*
 * A350779  Indices where Iccanobif numbers of the form A014258 decrease,
 *          i.e., numbers k such that A014258(k) < A014258(k-1).
 * https://oeis.org/A350779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search: iterate b(k) = A014258(k) = rev(b(k-1) + b(k-2)), b(0) =
 *            0, b(1) = 1, using exact decimal digit arrays, and list the k
 *            with b(k) < b(k-1)
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9559 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint8_t *A350779_pa = NULL, *A350779_pb = NULL, *A350779_pc = NULL;
static size_t A350779_la = 0, A350779_lb = 0, A350779_cap = 0;

static void A350779_grow(size_t need)
{
    if (need <= A350779_cap) return;
    size_t nc = A350779_cap ? A350779_cap : 64;
    while (nc < need) nc *= 2;
    uint8_t *t;
    t = (uint8_t *)realloc(A350779_pa, nc); if (!t) exit(1); A350779_pa = t;
    t = (uint8_t *)realloc(A350779_pb, nc); if (!t) exit(1); A350779_pb = t;
    t = (uint8_t *)realloc(A350779_pc, nc); if (!t) exit(1); A350779_pc = t;
    A350779_cap = nc;
}

/* compare little-endian digit arrays: -1, 0, 1 */
static int A350779_cmp(const uint8_t *x, size_t lx, const uint8_t *y, size_t ly)
{
    if (lx != ly) return lx < ly ? -1 : 1;
    for (size_t i = lx; i-- > 0;) {
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    static idx_t *found = NULL;
    static size_t cnt = 0, fcap = 0;
    static idx_t k = 1;
    static int init = 0;

    if (n < 1)
        return 0;

    if (!init) {
        A350779_grow(64);
        A350779_pa[0] = 0; A350779_la = 1;   /* b(0) = 0 */
        A350779_pb[0] = 1; A350779_lb = 1;   /* b(1) = 1 */
        init = 1;
    }

    while (cnt < (size_t)n) {
        size_t L = A350779_la > A350779_lb ? A350779_la : A350779_lb;
        A350779_grow(L + 2);
        uint8_t *x = A350779_pa, *y = A350779_pb, *c = A350779_pc;
        /* sum */
        unsigned carry = 0;
        for (size_t i = 0; i < L; i++) {
            unsigned d = carry;
            if (i < A350779_la) d += x[i];
            if (i < A350779_lb) d += y[i];
            c[i] = (uint8_t)(d % 10);
            carry = d / 10;
        }
        size_t lc = L;
        if (carry) c[lc++] = (uint8_t)carry;
        /* trim leading zeros of sum (keep at least one digit) */
        while (lc > 1 && c[lc - 1] == 0) lc--;
        /* reverse in place */
        for (size_t i = 0, j = lc - 1; i < j; i++, j--) {
            uint8_t t = c[i]; c[i] = c[j]; c[j] = t;
        }
        /* trim leading zeros of reversed value */
        while (lc > 1 && c[lc - 1] == 0) lc--;

        k++;
        if (A350779_cmp(c, lc, y, A350779_lb) < 0) {
            if (cnt == fcap) {
                size_t nc = fcap ? fcap * 2 : 64;
                idx_t *t = (idx_t *)realloc(found, nc * sizeof(idx_t));
                if (!t) exit(1);
                found = t;
                fcap = nc;
            }
            found[cnt++] = k;
        }
        /* rotate: a <- b, b <- c, c <- old a */
        A350779_pa = y; A350779_la = A350779_lb;
        A350779_pb = c; A350779_lb = lc;
        A350779_pc = x;
    }
    return (seq_t)found[n - 1];
}
