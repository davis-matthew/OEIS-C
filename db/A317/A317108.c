/*
 * A317108  Numbers missing from A317106.
 * https://oeis.org/A317108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not in A317106 (A317106 is finite:
 *            simulate the (1,3)-leaper walk on the spiral board until stuck,
 *            then take the n-th positive integer missing from the visited
 *            set)
 * Range:     u8 n <= 127, u16 n <= 61812, u32 n <= 67087, u64 n <= 67087,
 *            u128 n <= 67087, big any n
 * Validated: standard, pass, 80 terms through n = 9842 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define A317108_G 2048
#define A317108_OFF 1024

static unsigned char A317108_grid[A317108_G][A317108_G];
static unsigned long long *A317108_vals = NULL;
static size_t A317108_cnt = 0;
static bool A317108_ready = false;

static unsigned long long A317108_spiral(long long x, long long y)
{
    long long ax = x >= 0 ? x : -x;
    long long ay = y >= 0 ? y : -y;
    long long m = ax > ay ? ax : ay;
    long long t = (2 * m + 1) * (2 * m + 1);
    long long r;
    if (y == -m)
        r = t - (m - x);
    else if (x == -m)
        r = t - 2 * m - (y + m);
    else if (y == m)
        r = t - 4 * m - (x + m);
    else
        r = t - 6 * m - (-y + m);
    return (unsigned long long)r;
}

static int A317108_cmp(const void *p, const void *q)
{
    unsigned long long u = *(const unsigned long long *)p;
    unsigned long long v = *(const unsigned long long *)q;
    return (u > v) - (u < v);
}

static void A317108_init(void)
{
    static const int dx[8] = { 1, 1, -1, -1, 3, 3, -3, -3 };
    static const int dy[8] = { 3, -3, 3, -3, 1, -1, 1, -1 };
    size_t cap = 8192;
    int x = 0, y = 0;
    A317108_vals = (unsigned long long *)malloc(cap * sizeof(unsigned long long));
    A317108_grid[A317108_OFF][A317108_OFF] = 1;
    A317108_vals[A317108_cnt++] = 1;
    for (;;) {
        bool found = false;
        int bx = 0, by = 0;
        unsigned long long best = 0;
        for (int m = 0; m < 8; ++m) {
            int nx = x + dx[m], ny = y + dy[m];
            if (nx + A317108_OFF < 0 || nx + A317108_OFF >= A317108_G ||
                ny + A317108_OFF < 0 || ny + A317108_OFF >= A317108_G)
                continue;
            if (A317108_grid[nx + A317108_OFF][ny + A317108_OFF]) continue;
            unsigned long long c = A317108_spiral(nx, ny);
            if (!found || c < best) {
                found = true;
                best = c;
                bx = nx;
                by = ny;
            }
        }
        if (!found) break;
        if (A317108_cnt == cap) {
            cap *= 2;
            A317108_vals = (unsigned long long *)realloc(A317108_vals, cap * sizeof(unsigned long long));
        }
        A317108_vals[A317108_cnt++] = best;
        A317108_grid[bx + A317108_OFF][by + A317108_OFF] = 1;
        x = bx;
        y = by;
    }
    qsort(A317108_vals, A317108_cnt, sizeof(unsigned long long), A317108_cmp);
    A317108_ready = true;
}

seq_t a(idx_t n)
{
    if (!A317108_ready) A317108_init();
    unsigned long long r = (unsigned long long)n;
    for (size_t i = 0; i < A317108_cnt; ++i) {
        if (A317108_vals[i] <= r) r++;
        else break;
    }
    return (seq_t)r;
}
