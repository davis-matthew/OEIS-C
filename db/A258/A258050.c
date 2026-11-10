/*
 * A258050  Position of -n in A258047 after deleting the initial 0.
 * https://oeis.org/A258050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (index of the n-th negative term of A258047 (d(1)=0)) -
 *            1, i.e. the position of the n-th negative term after deleting
 *            the initial 0. Computed by generating A258047 with Rule 3
 *            (a(1)=1, d(1)=0) step by step
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 878 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static unsigned char *A258050_A = NULL;   /* value v occurs among a(k) */
static unsigned char *A258050_Dp = NULL;  /* positive h used as d */
static unsigned char *A258050_Dn = NULL;  /* negative -h used as d */
static long long A258050_M = 0;           /* capacity of the flag arrays */
static long long *A258050_neg = NULL;     /* d-indices of negative terms */
static long long A258050_ncnt = 0, A258050_ncap = 0;
static long long A258050_ak = 0;          /* current a(k) */
static long long A258050_k = 0;           /* current k */

static void A258050_ensure(long long need)
{
    if (need < A258050_M) return;
    long long nm = A258050_M ? A258050_M : 256;
    while (nm <= need) nm *= 2;
    unsigned char *a = (unsigned char *)realloc(A258050_A, (size_t)nm);
    unsigned char *p = (unsigned char *)realloc(A258050_Dp, (size_t)nm);
    unsigned char *d = (unsigned char *)realloc(A258050_Dn, (size_t)nm);
    if (!a || !p || !d) exit(1);
    for (long long i = A258050_M; i < nm; ++i) { a[i] = 0; p[i] = 0; d[i] = 0; }
    A258050_A = a; A258050_Dp = p; A258050_Dn = d;
    A258050_M = nm;
}

static bool A258050_inA(long long v)
{
    return v > 0 && v < A258050_M && A258050_A[v];
}

static void A258050_push_neg(long long idx)
{
    if (A258050_ncnt >= A258050_ncap) {
        long long nc = A258050_ncap ? A258050_ncap * 2 : 256;
        long long *q = (long long *)realloc(A258050_neg, (size_t)nc * sizeof(long long));
        if (!q) exit(1);
        A258050_neg = q;
        A258050_ncap = nc;
    }
    A258050_neg[A258050_ncnt++] = idx;
}

static void A258050_step(void)
{
    long long ak = A258050_ak, h;
    A258050_ensure(ak + 2);
    /* Step 1: least (most negative) h in (1-ak, 0) */
    for (h = 2 - ak; h <= -1; ++h) {
        if (!A258050_Dn[-h] && !A258050_inA(ak + h)) {
            A258050_ak = ak + h;
            ++A258050_k;
            A258050_Dn[-h] = 1;
            A258050_A[A258050_ak] = 1;
            A258050_push_neg(A258050_k);
            return;
        }
    }
    /* Step 2: least positive h not in D with ak - h not in A */
    for (h = 1; ; ++h) {
        A258050_ensure(h + 1);
        if (!A258050_Dp[h] && !A258050_inA(ak - h)) {
            A258050_ensure(ak + h + 1);
            A258050_ak = ak + h;
            ++A258050_k;
            A258050_Dp[h] = 1;
            A258050_A[A258050_ak] = 1;
            return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A258050_k == 0) {
        A258050_ensure(256);
        A258050_ak = 1;
        A258050_A[1] = 1;
        A258050_k = 1;          /* d(1) = 0 */
    }
    while (A258050_ncnt < (long long)n) A258050_step();
    return (seq_t)(A258050_neg[n - 1] - 1);
}
