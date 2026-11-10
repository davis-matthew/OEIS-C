/*
 * A355579  Numbers k such that A072079(k)/k sets a new record.
 * https://oeis.org/A355579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (3-smooth, k=2^i*3^j) where sigma(k)/k =
 *            3*(1-x)*(1-y), x=2^-(i+1), y=3^-(j+1), sets a new record (search
 *            over 3-smooth numbers in increasing order, minimizing x+y-x*y)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 43, u64 n <= 84, u128 n <= 168,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 4328 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static int *A355579_e2 = NULL, *A355579_e3 = NULL;
static size_t A355579_cnt = 0, A355579_cap = 0, A355579_p2 = 0, A355579_p3 = 0;
static int *A355579_r2 = NULL, *A355579_r3 = NULL;
static size_t A355579_nrec = 0, A355579_rcap = 0;
static long double A355579_best = 0;

static long double A355579_D(int i, int j) {
    long double x = ldexpl(1.0L, -(i + 1));
    long double y = powl(3.0L, -(long double)(j + 1));
    return x + y - x * y;
}

static void A355579_push_rec(int i, int j) {
    if (A355579_nrec == A355579_rcap) {
        A355579_rcap = A355579_rcap ? 2 * A355579_rcap : 1024;
        A355579_r2 = (int *)realloc(A355579_r2, A355579_rcap * sizeof(int));
        A355579_r3 = (int *)realloc(A355579_r3, A355579_rcap * sizeof(int));
    }
    A355579_r2[A355579_nrec] = i;
    A355579_r3[A355579_nrec] = j;
    A355579_nrec++;
}

static void A355579_add(int i, int j) {
    long double d;
    if (A355579_cnt == A355579_cap) {
        A355579_cap = A355579_cap ? 2 * A355579_cap : 4096;
        A355579_e2 = (int *)realloc(A355579_e2, A355579_cap * sizeof(int));
        A355579_e3 = (int *)realloc(A355579_e3, A355579_cap * sizeof(int));
    }
    A355579_e2[A355579_cnt] = i;
    A355579_e3[A355579_cnt] = j;
    A355579_cnt++;
    d = A355579_D(i, j);
    if (A355579_nrec == 0 || d < A355579_best * (1.0L - 1e-18L)) {
        A355579_best = d;
        A355579_push_rec(i, j);
    }
}

static void A355579_step(void) {
    const long double L2 = 0.69314718055994530942L, L3 = 1.09861228866810969140L;
    int a2, b2, a3, b3;
    if (A355579_cnt == 0) { A355579_add(0, 0); return; }
    a2 = A355579_e2[A355579_p2] + 1; b2 = A355579_e3[A355579_p2];
    a3 = A355579_e2[A355579_p3];     b3 = A355579_e3[A355579_p3] + 1;
    if (a2 == a3 && b2 == b3) {
        A355579_p2++; A355579_p3++;
        A355579_add(a2, b2);
    } else {
        long double v2 = a2 * L2 + b2 * L3;
        long double v3 = a3 * L2 + b3 * L3;
        if (v2 < v3) { A355579_p2++; A355579_add(a2, b2); }
        else { A355579_p3++; A355579_add(a3, b3); }
    }
}

seq_t a(idx_t n) {
    seq_t r = 1;
    int t, i, j;
    if (n < 1) return 0;
    while (A355579_nrec < (size_t)n) A355579_step();
    i = A355579_r2[n - 1];
    j = A355579_r3[n - 1];
    for (t = 0; t < i; t++) r *= 2;
    for (t = 0; t < j; t++) r *= 3;
    return r;
}
