/*
 * A343267  Numbers that are not the sum of four or fewer terms from A020330
 *          (not necessarily distinct).
 * https://oeis.org/A343267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th positive integer not representable as a sum of at
 *            most 4 terms of A020330 (terms x*(2^k+1), 2^(k-1)<=x<2^k);
 *            computed by a coin-count DP up to 700, since the sequence is
 *            finite (56 terms, last 686)
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A343267_LIM 700

static int A343267_ready = 0;
static unsigned A343267_list[128];
static int A343267_cnt = 0;

static void A343267_init(void)
{
    unsigned terms[64];
    int nt = 0;
    unsigned k, x;
    int best[A343267_LIM + 1];
    int s, i;

    for (k = 1; k < 12; k++) {
        for (x = (1u << (k - 1)); x < (1u << k); x++) {
            unsigned long long v = (unsigned long long)x * ((1ull << k) + 1);
            if (v <= A343267_LIM && nt < 64) terms[nt++] = (unsigned)v;
        }
    }
    best[0] = 0;
    for (s = 1; s <= A343267_LIM; s++) {
        best[s] = 100;
        for (i = 0; i < nt; i++) {
            if ((int)terms[i] <= s && best[s - terms[i]] + 1 < best[s])
                best[s] = best[s - terms[i]] + 1;
        }
        if (best[s] > 4 && A343267_cnt < 128)
            A343267_list[A343267_cnt++] = (unsigned)s;
    }
    A343267_ready = 1;
}

seq_t a(idx_t n)
{
    if (!A343267_ready) A343267_init();
    if (n < 1 || n > (idx_t)A343267_cnt) return 0;
    return (seq_t)A343267_list[n - 1];
}
