/*
 * A390290  Powers k^m, m > 1, such that k is in A303946 and are products of
 *          primorials.
 * https://oeis.org/A390290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number N = 2^e1*3^e2*5^e3*... with e1>=e2>=...>=1
 *            (A025487), g = gcd(e_i) >= 2 and e1 >= 2g (so the g-th root of N
 *            is not squarefree); found by enumerating A025487 up to a growing
 *            limit, then sorting
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 147, u64 n <= 1983,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 9536 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A390290_NP 60
static const unsigned A390290_primes[A390290_NP] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281
};

static seq_t A390290_L;
static seq_t *A390290_arr = NULL;   /* sorted terms <= A390290_done_L */
static size_t A390290_cnt = 0;
static size_t A390290_cap = 0;
static int A390290_exps[A390290_NP + 1];
static bool A390290_have = false;

static int A390290_gcd(int a, int b)
{
    while (b) { int t = a % b; a = b; b = t; }
    return a;
}

static void A390290_push(seq_t v)
{
    if (A390290_cnt >= A390290_cap) {
        size_t nc = A390290_cap ? A390290_cap * 2 : 256;
        seq_t *t = mem_resize(seq_t, A390290_arr, nc);
        if (!t) exit(EXIT_FAILURE);
        A390290_arr = t;
        A390290_cap = nc;
    }
    A390290_arr[A390290_cnt++] = v;
}

static void A390290_rec(int i, int maxe, seq_t cur)
{
    if (i > 0) {
        int g = 0;
        for (int j = 0; j < i; j++) {
            g = A390290_gcd(g, A390290_exps[j]);
            if (g == 1) break;
        }
        if (g >= 2 && A390290_exps[0] >= 2 * g)
            A390290_push(cur);
    }
    if (i >= A390290_NP) return;
    seq_t p = (seq_t)A390290_primes[i];
    if (cur > A390290_L / p) return;
    seq_t c = cur;
    for (int e = 1; e <= maxe; e++) {
        if (c > A390290_L / p) break;
        c = c * p;
        A390290_exps[i] = e;
        A390290_rec(i + 1, e, c);
    }
}

static void A390290_sort(seq_t *a, size_t n)
{
    size_t gap = 1;
    while (gap < n / 3) gap = gap * 3 + 1;
    for (; gap > 0; gap = (gap - 1) / 3) {
        for (size_t i = gap; i < n; i++) {
            seq_t t = a[i];
            size_t j = i;
            while (j >= gap && a[j - gap] > t) {
                a[j] = a[j - gap];
                j -= gap;
            }
            a[j] = t;
        }
        if (gap == 1) break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static seq_t lim = 0;
    if (!A390290_have) {
        lim = 255;
        A390290_have = true;
        A390290_cnt = 0;
        A390290_L = lim;
        A390290_rec(0, 1000, 1);
        A390290_sort(A390290_arr, A390290_cnt);
    }
    while ((idx_t)A390290_cnt < n) {
        seq_t nl = lim * 16 + 15;
        if ((nl >> 4) != lim) return 0; /* exceeds seq_t */
        lim = nl;
        A390290_cnt = 0;
        A390290_L = lim;
        A390290_rec(0, 1000, 1);
        A390290_sort(A390290_arr, A390290_cnt);
    }
    return A390290_arr[n - 1];
}
