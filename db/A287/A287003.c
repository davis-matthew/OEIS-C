/*
 * A287003  Positions of 0 in A287002.
 * https://oeis.org/A287003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in the fixed point of the morphism
 *            0->01, 1->20, 2->1 (generated incrementally, scanning the word)
 * Range:     u8 n <= 113, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9081 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287003_w = NULL;
static size_t A287003_len = 0, A287003_cap = 0, A287003_i = 0;
static uint64_t *A287003_z = NULL;
static size_t A287003_zc = 0, A287003_zcap = 0;

static void A287003_push(unsigned char s)
{
    if (A287003_len == A287003_cap) {
        A287003_cap = A287003_cap ? A287003_cap * 2 : 1024;
        A287003_w = (unsigned char *)realloc(A287003_w, A287003_cap);
    }
    A287003_w[A287003_len++] = s;
    if (s == 0) {
        if (A287003_zc == A287003_zcap) {
            A287003_zcap = A287003_zcap ? A287003_zcap * 2 : 1024;
            A287003_z = (uint64_t *)realloc(A287003_z, A287003_zcap * sizeof(uint64_t));
        }
        A287003_z[A287003_zc++] = (uint64_t)A287003_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A287003_len == 0) {
        /* The image of the first symbol 0 is "01", which starts with 0 itself;
           the word begins "01" and w[0] has already been expanded. */
        A287003_push(0);
        A287003_push(1);
        A287003_i = 1;
    }
    while (A287003_zc < (size_t)n) {
        unsigned char s = A287003_w[A287003_i++];
        if (s == 0) {
            A287003_push(0);
            A287003_push(1);
        } else if (s == 1) {
            A287003_push(2);
            A287003_push(0);
        } else {
            A287003_push(1);
        }
    }
    return (seq_t)A287003_z[n - 1];
}
