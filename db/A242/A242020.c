/*
 * A242020  Smallest index at which A242018 contains a run of n consecutive
 *          1s.
 * https://oeis.org/A242020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = least start s of a run of >= n ones in A242018. This is a
 *            search: the sequence is built by appending the last ceil(L/2)
 *            terms of the length-L prefix, and a new run of ones can only
 *            form at the junction of each append. Terms are read by recursive
 *            descent through the prefix lengths
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

#define A242020_CAP 5000
#define A242020_MAXN 20

static seq_t *A242020_L = NULL;      /* prefix lengths L[k] */
static seq_t *A242020_best = NULL;   /* found start index per run length */
static int A242020_found[A242020_MAXN + 1];
static int A242020_step = 0;
static bool A242020_init = false;

/* value of A242018 at index p inside prefix number k (prefix 0 = 0,0,1) */
static int A242020_bit(int k, seq_t p)
{
    while (k > 0) {
        if (p >= A242020_L[k - 1]) p -= (A242020_L[k - 1] + 1) / 2;
        k--;
    }
    return p == 2;
}

static void A242020_advance(void)
{
    int k = A242020_step;
    seq_t L = A242020_L[k];
    seq_t m = (L + 1) / 2;
    int t = 0, lead = 0;
    while (t < A242020_MAXN && (seq_t)t < L && A242020_bit(k, L - 1 - (seq_t)t)) t++;
    while (lead < A242020_MAXN && (seq_t)lead < m && A242020_bit(k, L - m + (seq_t)lead)) lead++;
    int r = t + lead;
    if (r > A242020_MAXN) r = A242020_MAXN;
    seq_t s = L - (seq_t)t;
    for (int len = 1; len <= r; len++) {
        if (!A242020_found[len]) {
            A242020_found[len] = 1;
            A242020_best[len] = s;
        }
    }
    A242020_L[k + 1] = L + m;
    A242020_step = k + 1;
}

seq_t a(idx_t n)
{
    if (!A242020_init) {
        A242020_L = mem_new(seq_t, A242020_CAP + 2);
        A242020_best = mem_new(seq_t, A242020_MAXN + 1);
        for (int i = 0; i <= A242020_MAXN; i++) A242020_found[i] = 0;
        A242020_L[0] = 3;
        A242020_step = 0;
        A242020_init = true;
    }
    if (n < 1 || n > (idx_t)A242020_MAXN) return 0;
    int len = (int)n;
    while (!A242020_found[len] && A242020_step < A242020_CAP) A242020_advance();
    if (A242020_found[len]) return A242020_best[len];
    return 0;
}
