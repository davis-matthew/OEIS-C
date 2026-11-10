/*
 * A370219  Irregular triangle T(n,k) read by rows: row n lists run-length
 *          encoding d_k values (see comments) for the properly nested string
 *          of parentheses encoded by A063171(n).
 * https://oeis.org/A370219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row n (block m has Catalan(m)*m terms) lists d_1..d_m of the
 *            r-th Dyck word of semilength m in ascending binary order (1="(",
 *            0=")"); d_k = number of ")" between the k-th "(" and the
 *            (k+1)-th "(" (or the end). Computed by ballot-path unranking
 * Range:     u8 n <= 15521, u16 n <= 15521, u32 n <= 15521, u64 n <= 15521,
 *            u128 n <= 15521, big any n
 * Validated: standard, pass, 107 terms through n = 9807 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A370219_u128;

static A370219_u128 A370219_W[70][72];
static int A370219_init = 0;

static void A370219_setup(void)
{
    int r, b;
    if (A370219_init) return;
    for (r = 0; r < 70; ++r)
        for (b = 0; b < 72; ++b)
            A370219_W[r][b] = 0;
    A370219_W[0][0] = 1;
    for (r = 1; r < 70; ++r)
        for (b = 0; b < 71; ++b) {
            A370219_u128 v = A370219_W[r - 1][b + 1];
            if (b > 0) v += A370219_W[r - 1][b - 1];
            A370219_W[r][b] = v;
        }
    A370219_init = 1;
}

seq_t a(idx_t n)
{
    A370219_setup();
    A370219_u128 n0 = (A370219_u128)(n - 1);
    int m = 1;
    for (;;) {
        A370219_u128 sz = A370219_W[2 * m][0] * (A370219_u128)m;
        if (n0 < sz) break;
        n0 -= sz;
        ++m;
    }
    A370219_u128 r = n0 / (A370219_u128)m;
    int k = (int)(n0 % (A370219_u128)m) + 1;

    int bal = 0, opens = 0;
    int cnt = 0;
    for (int pos = 0; pos < 2 * m; ++pos) {
        int rem = 2 * m - pos;
        int closeIt = 0;
        if (bal > 0) {
            A370219_u128 c0 = A370219_W[rem - 1][bal - 1];
            if (r < c0) closeIt = 1;
            else r -= c0;
        }
        if (closeIt) {
            --bal;
            if (opens == k) ++cnt;
        } else {
            ++bal;
            ++opens;
        }
    }
    return (seq_t)cnt;
}
