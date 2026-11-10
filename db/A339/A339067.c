/*
 * A339067  Triangle read by rows: T(n,k) is the number of linear forests with
 *          n nodes and k rooted trees.
 * https://oeis.org/A339067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = [x^n] t(x)^k where t(x) = Sum_{i>=1} A000081(i) x^i;
 *            flattened by rows, n -> (row r, column k)
 * Range:     u8 n <= 36, u16 n <= 93, u32 n <= 302, u64 n <= 1038,
 *            u128 n <= 3830, big any n
 * Validated: standard, pass, 82 terms through n = 9187 (2026-11-08)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

#define A81_MAX 32768

static seq_t A81_T[A81_MAX + 2];
static seq_t A81_P[A81_MAX + 2];
static seq_t A81_M[A81_MAX + 2];
static idx_t A81_cap = 0;

static void a81_build(idx_t N)
{
    idx_t L = N;
    seq_t *P = A81_P;
    seq_t *M = A81_M;
    seq_t *T = A81_T;
    idx_t i, k, m, j;

    for (i = 0; i <= L; i++) { P[i] = 0; M[i] = 0; }
    for (i = 0; i <= N + 1; i++) T[i] = 0;
    P[0] = 1;
    T[0] = 0;
    T[1] = 1;

    for (k = 1; k < N; k++) {
        seq_t c = T[k];
        idx_t J = (L - 1) / k;
        M[0] = 1;
        for (j = 1; j <= J; j++) {
            unsigned long long r = (unsigned long long)(M[j - 1] % (seq_t)j);
            unsigned long long g = gcd_u64(r, (unsigned long long)j);
            seq_t x = M[j - 1] / (seq_t)g;
            seq_t y = (c + (seq_t)(j - 1)) / (seq_t)((unsigned long long)j / g);
            M[j] = x * y;
        }
        for (m = L - 1; m >= k; m--) {
            seq_t sum = P[m];
            idx_t r = m - k;
            for (j = 1;; j++) {
                sum += M[j] * P[r];
                if (r < k) break;
                r -= k;
            }
            P[m] = sum;
        }
        T[k + 1] = P[k];
    }
}

static seq_t A000081_a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A81_MAX) n = A81_MAX;
    if (n > A81_cap) {
        idx_t nc = A81_cap + A81_cap / 2;
        if (nc < n) nc = n;
        if (nc < 16) nc = 16;
        if (nc > A81_MAX) nc = A81_MAX;
        a81_build(nc);
        A81_cap = nc;
    }
    return A81_T[n];
}

seq_t a(idx_t n)
{
    idx_t r = 1, rem = n, i, m, j, kk;
    while (rem > r) { rem -= r; r++; }
    idx_t k = rem;

    seq_t *t = mem_new(seq_t, r + 2);
    seq_t *P = mem_new(seq_t, r + 2);
    seq_t *Q = mem_new(seq_t, r + 2);
    for (i = 1; i <= r; i++) { t[i] = A000081_a(i); P[i] = t[i]; }

    for (kk = 2; kk <= k; kk++) {
        for (m = 0; m <= r; m++) Q[m] = 0;
        for (m = kk; m <= r; m++) {
            seq_t s = 0;
            for (j = 1; j + (kk - 1) <= m; j++) {
                s += t[j] * P[m - j];
            }
            Q[m] = s;
        }
        seq_t *tmp = P; P = Q; Q = tmp;
    }
    seq_t res = P[r];
    mem_free(t);
    mem_free(P);
    mem_free(Q);
    return res;
}
