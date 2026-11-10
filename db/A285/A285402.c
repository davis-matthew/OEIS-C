/*
 * A285402  Positions of 1 in A285177; complement of A285401.
 * https://oeis.org/A285402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285177, the 0-limiting word of
 *            the morphism 0->11, 1->001 (built by iterating the morphism an
 *            even number of times)
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9553 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint8_t *A285402_bits = NULL;
static size_t A285402_len = 0;
static size_t A285402_ones = 0;
static unsigned A285402_iter = 0;

static void A285402_step(void)
{
    size_t nl = 0, i, p = 0, ones = 0;
    uint8_t *nb;
    for (i = 0; i < A285402_len; ++i)
        nl += A285402_bits[i] ? 3 : 2;
    nb = (uint8_t *)malloc(nl);
    for (i = 0; i < A285402_len; ++i) {
        if (A285402_bits[i]) {
            nb[p++] = 0; nb[p++] = 0; nb[p++] = 1;
            ones++;
        } else {
            nb[p++] = 1; nb[p++] = 1;
            ones += 2;
        }
    }
    free(A285402_bits);
    A285402_bits = nb;
    A285402_len = nl;
    A285402_ones = ones;
    A285402_iter++;
}

seq_t a(idx_t n)
{
    size_t i, cnt = 0;
    if (n < 1) return (seq_t)0;
    if (A285402_bits == NULL) {
        A285402_bits = (uint8_t *)malloc(1);
        A285402_bits[0] = 0;
        A285402_len = 1;
        A285402_ones = 0;
        A285402_iter = 0;
    }
    while ((A285402_iter & 1U) || (idx_t)A285402_ones < n)
        A285402_step();
    for (i = 0; i < A285402_len; ++i) {
        if (A285402_bits[i]) {
            cnt++;
            if ((idx_t)cnt == n)
                return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
