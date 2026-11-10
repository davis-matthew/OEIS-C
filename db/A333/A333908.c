/*
 * A333908  Position of n-th 0 in A030604.
 * https://oeis.org/A333908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position (offset 0) of the n-th 0 in the base-6
 *            concatenation of Fibonacci numbers F(0),F(1),... (A030604);
 *            computed by scanning digits with a base-6 bignum Fibonacci
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *A030604_fa = NULL, *A030604_fb = NULL;
static size_t A030604_la = 0, A030604_lb = 0, A030604_cap = 0;
static uint64_t *A030604_z = NULL;
static size_t A030604_zn = 0, A030604_zc = 0;
static uint64_t A030604_pos = 0;
static bool A030604_init = false;

static void A030604_grow(size_t need)
{
    if (need <= A030604_cap) return;
    size_t nc = A030604_cap ? A030604_cap * 2 : 64;
    while (nc < need) nc *= 2;
    A030604_fa = (unsigned char *)realloc(A030604_fa, nc);
    A030604_fb = (unsigned char *)realloc(A030604_fb, nc);
    A030604_cap = nc;
}

static void A030604_step(void)
{
    /* emit digits of fa, most significant first */
    for (size_t i = A030604_la; i > 0; i--) {
        if (A030604_fa[i - 1] == 0) {
            if (A030604_zn == A030604_zc) {
                size_t nc = A030604_zc ? A030604_zc * 2 : 64;
                A030604_z = (uint64_t *)realloc(A030604_z, nc * sizeof(uint64_t));
                A030604_zc = nc;
            }
            A030604_z[A030604_zn++] = A030604_pos;
        }
        A030604_pos++;
    }
    /* fa += fb, then swap */
    size_t m = A030604_la > A030604_lb ? A030604_la : A030604_lb;
    A030604_grow(m + 2);
    unsigned carry = 0;
    for (size_t i = 0; i < m; i++) {
        unsigned s = carry;
        if (i < A030604_la) s += A030604_fa[i];
        if (i < A030604_lb) s += A030604_fb[i];
        A030604_fa[i] = (unsigned char)(s % 6);
        carry = s / 6;
    }
    size_t ln = m;
    if (carry) A030604_fa[ln++] = (unsigned char)carry;
    unsigned char *t = A030604_fa; A030604_fa = A030604_fb; A030604_fb = t;
    A030604_la = A030604_lb;
    A030604_lb = ln;
}

seq_t a(idx_t n)
{
    if (!A030604_init) {
        A030604_init = true;
        A030604_grow(64);
        A030604_fa[0] = 0; A030604_la = 1;  /* F(0) = 0 */
        A030604_fb[0] = 1; A030604_lb = 1;  /* F(1) = 1 */
    }
    while ((uint64_t)A030604_zn < (uint64_t)n)
        A030604_step();
    return (seq_t)A030604_z[(size_t)(n - 1)];
}
