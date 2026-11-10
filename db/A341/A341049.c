/*
 * A341049  Irregular triangle read by rows T(n,k) in which row n lists the
 *          terms of n-th row of A336811 in nondecreasing order.
 * https://oeis.org/A341049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row n of A336811 sorted; the value v (1..n) appears
 *            p(s-1)-p(s-2) times with s=n+1-v (s=1: once), p = A000041;
 *            computed by locating row then counting
 * Range:     u8 n <= 11732, u16 n <= 11732, u32 n <= 11732, u64 n <= 11732,
 *            u128 n <= 11732, big any n
 * Validated: standard, pass, 113 terms through n = 8597 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A341049_PM 700

static unsigned __int128 A341049_p[A341049_PM + 1];
static bool A341049_init = false;

static void A341049_setup(void)
{
    if (A341049_init) return;
    for (int i = 0; i <= A341049_PM; i++) A341049_p[i] = 0;
    A341049_p[0] = 1;
    for (int i = 1; i <= A341049_PM; i++)
        for (int j = i; j <= A341049_PM; j++)
            A341049_p[j] += A341049_p[j - i];
    A341049_init = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A341049_setup();
    unsigned __int128 target = (unsigned __int128)n;
    unsigned __int128 cum = 0;
    int r = 1;
    for (;;) {
        unsigned __int128 len = A341049_p[r - 1];
        if (cum + len >= target) break;
        cum += len;
        r++;
    }
    unsigned __int128 off = target - cum;
    for (int v = 1; v <= r; v++) {
        int s = r + 1 - v;
        unsigned __int128 c = (s == 1) ? (unsigned __int128)1
                                       : A341049_p[s - 1] - A341049_p[s - 2];
        if (off <= c) return (seq_t)v;
        off -= c;
    }
    return 0;
}
