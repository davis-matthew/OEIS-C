/*
 * A370221  Irregular triangle T(n,k) read by rows: row n lists the values
 *          encoding a permutation (see comments) related to the properly
 *          nested string of parentheses encoded by A063171(n).
 * https://oeis.org/A370221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Irregular triangle read by rows; row n is the Dyck word
 *            A063171(n) (1="(" , 0=")", ascending as binary numbers). T(n,k)
 *            = index of the "(" matched by the k-th ")". Computed by
 *            unranking the word via ballot-path counts
 * Range:     u8 n <= 15521, u16 n <= 15521, u32 n <= 15521, u64 n <= 15521,
 *            u128 n <= 15521, big any n
 * Validated: standard, pass, 107 terms through n = 9972 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A370221_u128;

static A370221_u128 A370221_W[90][92];
static int A370221_init = 0;

static void A370221_setup(void)
{
    int r, b;
    if (A370221_init) return;
    for (r = 0; r < 90; ++r)
        for (b = 0; b < 92; ++b)
            A370221_W[r][b] = 0;
    A370221_W[0][0] = 1;
    for (r = 1; r < 90; ++r)
        for (b = 0; b < 91; ++b) {
            A370221_u128 v = A370221_W[r - 1][b + 1];
            if (b > 0) v += A370221_W[r - 1][b - 1];
            A370221_W[r][b] = v;
        }
    A370221_init = 1;
}

seq_t a(idx_t n)
{
    A370221_setup();
    A370221_u128 n0 = (A370221_u128)(n - 1);
    int m = 1;
    A370221_u128 cat;
    for (;;) {
        cat = A370221_W[2 * m][0];
        cat = cat / 1;
        /* W[2m][0] is the Catalan number C_m */
        A370221_u128 sz = cat * (A370221_u128)m;
        if (n0 < sz) break;
        n0 -= sz;
        ++m;
    }
    A370221_u128 r = n0 / (A370221_u128)m;
    int k = (int)(n0 % (A370221_u128)m) + 1;

    int stack[64];
    int sp = 0, nextOpen = 1, closes = 0, bal = 0;
    for (int pos = 0; pos < 2 * m; ++pos) {
        int rem = 2 * m - pos;
        int closeIt = 0;
        if (bal > 0) {
            A370221_u128 c0 = A370221_W[rem - 1][bal - 1];
            if (r < c0) closeIt = 1;
            else r -= c0;
        }
        if (closeIt) {
            int v = stack[--sp];
            --bal;
            ++closes;
            if (closes == k) return (seq_t)v;
        } else {
            stack[sp++] = nextOpen++;
            ++bal;
        }
    }
    return (seq_t)0;
}
