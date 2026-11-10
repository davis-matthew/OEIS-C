/*
 * A307162  a(n) is the smallest k such that A319100(k) = A025610(n).
 * https://oeis.org/A307162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with A025610(n) = 2^i*6^j, p(j)=A002476(j), q(i)=A007528(i),
 *            P(j)=prod_{k<=j} p(k), Q(i)=prod_{k<=i} q(k): i=0: 1, 7,
 *            9*P(j-1); i=1: 3, 21, 36*P(j-1); i=2: 8, 56, 72*P(j-1)
 *            (j=0,1,>=2); i>=3: 24*Q(i-3) if j=0, else
 *            P(j-1)*8*Q(i-3)*min(9*q(i-2), 3*p(j)). The n-th term 2^i*6^j of
 *            A025610 is found b...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 54, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 978 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct { double v; int i; int j; } A307162_pair;

static int A307162_cmp(const void *x, const void *y)
{
    double a = ((const A307162_pair *)x)->v, b = ((const A307162_pair *)y)->v;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

static int A307162_isprime(uint64_t m)
{
    if (m < 2) return 0;
    for (uint64_t d = 2; d * d <= m; d++)
        if (m % d == 0) return 0;
    return 1;
}

/* k-th prime congruent to r mod 6 (r = 1 or 5), k >= 1 */
static uint64_t A307162_prime_mod6(int k, int r)
{
    uint64_t c = (r == 1) ? 7 : 5;
    int cnt = 0;
    for (;; c += 6) {
        if (A307162_isprime(c)) {
            cnt++;
            if (cnt == k) return c;
        }
    }
}

static seq_t A307162_P(int j)
{
    seq_t r = 1;
    for (int k = 1; k <= j; k++) r *= (seq_t)A307162_prime_mod6(k, 1);
    return r;
}

static seq_t A307162_Q(int i)
{
    seq_t r = 1;
    for (int k = 1; k <= i; k++) r *= (seq_t)A307162_prime_mod6(k, 5);
    return r;
}

static void A307162_find(idx_t n, int *ei, int *ej)
{
    const double l2 = log(2.0), l6 = log(6.0);
    double L = 10.0;
    for (;;) {
        size_t cap = 0, cnt = 0;
        A307162_pair *arr = NULL;
        for (int j = 0; j * l6 <= L; j++) {
            for (int i = 0; i * l2 + j * l6 <= L; i++) {
                if (cnt == cap) {
                    cap = cap ? cap * 2 : 64;
                    arr = (A307162_pair *)realloc(arr, cap * sizeof(A307162_pair));
                }
                arr[cnt].v = i * l2 + j * l6;
                arr[cnt].i = i;
                arr[cnt].j = j;
                cnt++;
            }
        }
        if ((idx_t)cnt >= n) {
            qsort(arr, cnt, sizeof(A307162_pair), A307162_cmp);
            *ei = arr[n - 1].i;
            *ej = arr[n - 1].j;
            free(arr);
            return;
        }
        free(arr);
        L *= 1.5;
    }
}

seq_t a(idx_t n)
{
    int i, j;
    A307162_find(n, &i, &j);

    if (i <= 2) {
        static const int base[3][3] = { {1, 7, 9}, {3, 21, 36}, {8, 56, 72} };
        if (j == 0) return (seq_t)base[i][0];
        if (j == 1) return (seq_t)base[i][1];
        return (seq_t)base[i][2] * A307162_P(j - 1);
    }

    if (j == 0)
        return (seq_t)24 * A307162_Q(i - 3);

    seq_t x = (seq_t)9 * (seq_t)A307162_prime_mod6(i - 2, 5);
    seq_t y = (seq_t)3 * (seq_t)A307162_prime_mod6(j, 1);
    seq_t m = (x < y) ? x : y;
    return A307162_P(j - 1) * (seq_t)8 * A307162_Q(i - 3) * m;
}
