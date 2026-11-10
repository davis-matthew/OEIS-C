/*
 * A364863  Number of iterations of x -> x + min { k in A036301 | k > x }
 *          until an element of A036301 is reached, or -1 if this never
 *          happens, starting with n.
 * https://oeis.org/A364863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of x -> x + (least element of
 *            A036301 greater than x) starting at x=n until x is in A036301
 *            (a(n)=0 if n is in A036301); computed by direct iteration with a
 *            digit-DP "next balanced number" search, -1 if not reached (or
 *            overflow / iteration cap)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Validated: partial, pass, 2 terms through n = 1 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define A364863_MAXD 64
#define A364863_OFF 600
#define A364863_W 1201

static unsigned char A364863_reach[A364863_MAXD + 1][A364863_W];
static bool A364863_init = false;

static int A364863_contrib(int d)
{
    return (d & 1) ? d : -d;
}

static void A364863_build(void)
{
    int r, t, d;
    if (A364863_init) return;
    for (r = 0; r <= A364863_MAXD; r++)
        for (t = 0; t < A364863_W; t++)
            A364863_reach[r][t] = 0;
    A364863_reach[0][A364863_OFF] = 1;
    for (r = 1; r <= A364863_MAXD; r++) {
        for (t = 0; t < A364863_W; t++) {
            if (!A364863_reach[r - 1][t]) continue;
            for (d = 0; d <= 9; d++) {
                int nt = t + A364863_contrib(d);
                if (nt >= 0 && nt < A364863_W)
                    A364863_reach[r][nt] = 1;
            }
        }
    }
    A364863_init = true;
}

/* Fill digits D (most significant first, length L) with x's digits; returns false if too long. */
static bool A364863_digits(seq_t x, int *D, int *L)
{
    int tmp[A364863_MAXD];
    int n = 0, i;
    while (x > 0) {
        if (n >= A364863_MAXD - 4) return false;
        tmp[n++] = (int)(x % 10);
        x /= 10;
    }
    for (i = 0; i < n; i++) D[i] = tmp[n - 1 - i];
    *L = n;
    return true;
}

/* Smallest balanced number > x as digit string; returns padded length P via *Pout, digits in R. */
static bool A364863_next(const int *Dx, int L, int *R, int *Pout)
{
    int D[A364863_MAXD];
    int pre[A364863_MAXD + 1];
    int P = L + 3, i, d, k;
    for (i = 0; i < P - L; i++) D[i] = 0;
    for (i = 0; i < L; i++) D[P - L + i] = Dx[i];
    pre[0] = 0;
    for (i = 0; i < P; i++) pre[i + 1] = pre[i] + A364863_contrib(D[i]);
    for (i = P - 1; i >= 0; i--) {
        int r = P - i - 1;
        for (d = D[i] + 1; d <= 9; d++) {
            int B = pre[i] + A364863_contrib(d);
            int target = -B + A364863_OFF;
            if (target < 0 || target >= A364863_W) continue;
            if (!A364863_reach[r][target]) continue;
            /* construct */
            for (k = 0; k < i; k++) R[k] = D[k];
            R[i] = d;
            {
                int need = -B;
                int pos;
                for (pos = i + 1; pos < P; pos++) {
                    int rem = P - pos - 1, e;
                    for (e = 0; e <= 9; e++) {
                        int nt = need - A364863_contrib(e) + A364863_OFF;
                        if (nt >= 0 && nt < A364863_W && A364863_reach[rem][nt]) {
                            R[pos] = e;
                            need -= A364863_contrib(e);
                            break;
                        }
                    }
                }
            }
            *Pout = P;
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    long long count = 0;
    A364863_build();
    for (;;) {
        int D[A364863_MAXD], R[A364863_MAXD];
        int L = 0, P = 0, i, s = 0;
        seq_t y = 0, sum;
        if (!A364863_digits(x, D, &L)) return (seq_t)0 - (seq_t)1;
        for (i = 0; i < L; i++) s += A364863_contrib(D[i]);
        if (s == 0) return (seq_t)count;
        if (count >= 100000) return (seq_t)0 - (seq_t)1;
        if (!A364863_next(D, L, R, &P)) return (seq_t)0 - (seq_t)1;
        for (i = 0; i < P; i++) {
            seq_t acc = y * 10 + (seq_t)R[i];
            if (acc / 10 != y) return (seq_t)0 - (seq_t)1;
            y = acc;
        }
        sum = x + y;
        if (sum < x) return (seq_t)0 - (seq_t)1;
        x = sum;
        count++;
    }
}
