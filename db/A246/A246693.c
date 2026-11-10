/*
 * A246693  Integers of the form A000129(k)/k.
 * https://oeis.org/A246693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000129(k_n)/k_n, where k_1 < k_2 < ... are the k >= 1 *
 *            with k | A000129(k) (1, 2, 4, 8, 12, 16, 24, 32, 36, 48, 56, 64,
 *            72, 84, * 96, ...). Search: test each k with a fast-doubling
 *            Pell evaluation mod k, * then compute A000129(k) exactly and
 *            divide by k. * * Note: the term at n = 14 come...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 19 terms through n = 20 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A246693_ks = 0;
static size_t A246693_cnt = 0, A246693_cap = 0;
static uint64_t A246693_next = 1;

/* returns nonzero iff k divides A000129(k) */
static int A246693_divides(uint64_t k)
{
    unsigned __int128 x = 0, y = 1 % k, K = k;
    int b;
    for (b = 63; b >= 0; b--) {
        /* (x,y) = (P(m), P(m+1)) -> (P(2m), P(2m+1)) */
        unsigned __int128 t = (2 * y + 2 * (K - x)) % K;   /* P(m+1)+P(m-1) */
        unsigned __int128 x2 = x * t % K;
        unsigned __int128 y2 = (y * y + x * x) % K;
        if ((k >> b) & 1) {
            x = y2;
            y = (2 * y2 + x2) % K;
        } else {
            x = x2;
            y = y2;
        }
    }
    return x == 0;
}

static seq_t A246693_pell_div(uint64_t k)
{
    seq_t x = 0, y = 1, t;
    uint64_t i;
    for (i = 0; i < k; i++) {
        t = y;
        y = 2 * y + x;
        x = t;
    }
    return x / (seq_t)k;
}

seq_t a(idx_t n)
{
    while (A246693_cnt < (size_t)n) {
        if (A246693_divides(A246693_next)) {
            if (A246693_cnt == A246693_cap) {
                A246693_cap = A246693_cap ? A246693_cap * 2 : 64;
                A246693_ks = (uint64_t *)realloc(A246693_ks, A246693_cap * sizeof(uint64_t));
            }
            A246693_ks[A246693_cnt++] = A246693_next;
        }
        A246693_next++;
    }
    return A246693_pell_div(A246693_ks[n - 1]);
}
