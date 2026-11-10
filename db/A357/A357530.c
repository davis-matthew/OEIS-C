/*
 * A357530  Reverse run lengths in binary expansions of terms of A031443: for
 *          n > 0, a(n) is the unique k such that A031443(k) =
 *          A056539(A031443(n)); a(0) = 0.
 * https://oeis.org/A357530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that A031443(k) = A056539(A031443(n)); a(0)=0.
 *            Computed by combinatorial unranking/ranking of balanced binary
 *            strings
 * Range:     u8 n <= 175, u16 n <= 8788, u32 n <= 8788, u64 n <= 8788,
 *            u128 n <= 8788, big any n
 * Validated: standard, pass, 81 terms through n = 8724 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 a357530_u128;

static a357530_u128 A357530_C[72][72];
static int A357530_init = 0;

static void A357530_setup(void)
{
    if (A357530_init) return;
    for (int i = 0; i < 72; i++) {
        A357530_C[i][0] = 1;
        for (int j = 1; j < 72; j++)
            A357530_C[i][j] = 0;
        for (int j = 1; j <= i; j++)
            A357530_C[i][j] = A357530_C[i-1][j-1] + (j <= i-1 ? A357530_C[i-1][j] : 0);
    }
    A357530_init = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A357530_setup();

    /* find length class */
    a357530_u128 r = (a357530_u128)n - 1;
    int L = 1;
    a357530_u128 base = 0;
    for (;;) {
        a357530_u128 c = A357530_C[2*L-1][L-1];
        if (r < c) break;
        r -= c;
        base += c;
        L++;
    }

    int len = 2 * L;
    int b[80], d[80];
    b[0] = 1;
    int ones = L - 1;
    for (int i = 1; i < len; i++) {
        int rem = len - i;
        a357530_u128 c0 = A357530_C[rem-1][ones];
        if (r < c0) {
            b[i] = 0;
        } else {
            r -= c0;
            b[i] = 1;
            ones--;
        }
    }

    /* A056539: reverse, complement if run count even */
    int runs = 1;
    for (int i = 1; i < len; i++)
        if (b[i] != b[i-1]) runs++;
    for (int i = 0; i < len; i++) {
        int v = b[len - 1 - i];
        d[i] = (runs % 2 == 0) ? 1 - v : v;
    }

    /* rank d among balanced numbers of the same length */
    a357530_u128 rk = 0;
    ones = L - 1;
    for (int i = 1; i < len; i++) {
        int rem = len - i;
        if (d[i]) {
            rk += A357530_C[rem-1][ones];
            ones--;
        }
    }
    return (seq_t)(base + rk + 1);
}
