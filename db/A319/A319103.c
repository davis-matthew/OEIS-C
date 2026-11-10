/*
 * A319103  a(n) is the least k > 0 such that A318928(k) = n.
 * https://oeis.org/A319103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = least k > 0 with A318928(k) = n, found by an exact
 *            search. It builds the RUNS chain top-down from a single element
 *            [c]. At each level it picks run values w_i (adjacent values
 *            distinct), so RUNS of the new list gives the previous one. It
 *            deepens the bit-length bound B until a solution appea...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int A319103_Bmax;
static int **A319103_lv;     /* A319103_lv[k]: list at level k            */
static int **A319103_suf;    /* suffix sums of list at level k            */
static int **A319103_w;      /* chosen run values for building level k-1  */
static int *A319103_len;
static int *A319103_sum;
static unsigned char *A319103_bits, *A319103_best;
static int A319103_bestB;
static bool A319103_have;

/* lower bound on final bit length from a level-j list of length l, sum s */
static long A319103_lb(int j, long l, long s)
{
    while (j > 1) {
        long ns = s + l / 2;
        l = s;
        s = ns;
        j--;
        if (s > A319103_Bmax) return s;
    }
    return s;
}

static void A319103_emit(void)
{
    int *L = A319103_lv[1];
    int r = A319103_len[1], B = A319103_sum[1];
    int i, c, pos = 0;
    unsigned char bit = 1;
    if (B > A319103_Bmax) return;
    for (i = 0; i < r; i++) {
        for (c = 0; c < L[i]; c++) A319103_bits[pos++] = bit;
        bit ^= 1;
    }
    if (!A319103_have || B < A319103_bestB ||
        (B == A319103_bestB && memcmp(A319103_bits, A319103_best, (size_t)B) < 0)) {
        memcpy(A319103_best, A319103_bits, (size_t)B);
        A319103_bestB = B;
        A319103_have = true;
    }
}

static void A319103_choose(int k, int t, long P, int prev)
{
    int *L = A319103_lv[k];
    int r = A319103_len[k];
    int *suf = A319103_suf[k];
    int *w = A319103_w[k];
    int wv;
    if (t == r) {
        int *N = A319103_lv[k - 1];
        int i, c, pos = 0;
        for (i = 0; i < r; i++)
            for (c = 0; c < L[i]; c++) N[pos++] = w[i];
        A319103_len[k - 1] = pos;
        A319103_sum[k - 1] = (int)P;
        if (k - 1 == 1) {
            A319103_emit();
        } else {
            int j, *S = A319103_suf[k - 1];
            S[pos] = 0;
            for (j = pos - 1; j >= 0; j--) S[j] = S[j + 1] + N[j];
            A319103_choose(k - 1, 0, 0, 0);
        }
        return;
    }
    for (wv = 1;; wv++) {
        long Pn, sl;
        if (wv == prev) continue;
        Pn = P + (long)wv * L[t];
        sl = Pn + suf[t + 1] + (r - t - 1) / 2;
        if (A319103_lb(k - 1, A319103_sum[k], sl) > A319103_Bmax) break;
        w[t] = wv;
        A319103_choose(k, t + 1, Pn, wv);
    }
}

seq_t a(idx_t n)
{
    int N, Bmax, k, c;
    seq_t v = 0;
    if (n == 0) return (seq_t)1;
    N = (int)n;
    A319103_have = false;
    for (Bmax = 2; !A319103_have; Bmax++) {
        A319103_Bmax = Bmax;
        A319103_lv = (int **)malloc(sizeof(int *) * (size_t)(N + 1));
        A319103_suf = (int **)malloc(sizeof(int *) * (size_t)(N + 1));
        A319103_w = (int **)malloc(sizeof(int *) * (size_t)(N + 1));
        A319103_len = (int *)malloc(sizeof(int) * (size_t)(N + 1));
        A319103_sum = (int *)malloc(sizeof(int) * (size_t)(N + 1));
        for (k = 0; k <= N; k++) {
            A319103_lv[k] = (int *)malloc(sizeof(int) * (size_t)(Bmax + 2));
            A319103_suf[k] = (int *)malloc(sizeof(int) * (size_t)(Bmax + 3));
            A319103_w[k] = (int *)malloc(sizeof(int) * (size_t)(Bmax + 2));
        }
        A319103_bits = (unsigned char *)malloc((size_t)Bmax + 1);
        A319103_best = (unsigned char *)malloc((size_t)Bmax + 1);

        for (c = 2; c <= Bmax; c++) {
            if (A319103_lb(N, 1, c) > Bmax) break;
            A319103_lv[N][0] = c;
            A319103_len[N] = 1;
            A319103_sum[N] = c;
            if (N == 1) {
                A319103_emit();
            } else {
                A319103_suf[N][1] = 0;
                A319103_suf[N][0] = c;
                A319103_choose(N, 0, 0, 0);
            }
        }

        if (A319103_have) {
            int i;
            for (i = 0; i < A319103_bestB; i++)
                v = v * 2 + (seq_t)A319103_best[i];
        }
        for (k = 0; k <= N; k++) {
            free(A319103_lv[k]);
            free(A319103_suf[k]);
            free(A319103_w[k]);
        }
        free(A319103_lv);
        free(A319103_suf);
        free(A319103_w);
        free(A319103_len);
        free(A319103_sum);
        free(A319103_bits);
        free(A319103_best);
    }
    return v;
}
