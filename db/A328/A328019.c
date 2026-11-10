/*
 * A328019  Positions of 1 in A328018.
 * https://oeis.org/A328019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; a(n+1) = A092433(a(n)), where A092433(k) is the least
 *            x * with x - nonF(x) = k. nonF(x) counts the m <= x that are not
 *            divisible by 7 * and have no digit 7; it is computed with a
 *            digit DP. x is found by the * fixed-point iteration x <- k +
 *            nonF(x), starting from x = k
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 31, u64 n <= 138,
 *            u128 n <= 1340, big any n
 * Validated: standard, pass, 47 terms through n = 8940 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* C[L*7+s]: number of L-digit strings (leading zeros allowed) with no digit 7
 * and value == s (mod 7). T[L] = 9^L. */
static seq_t *A328019_C = NULL;
static seq_t *A328019_T = NULL;
static int A328019_Lmax = -1;

static void A328019_ensure(int L)
{
    if (L <= A328019_Lmax) return;
    A328019_C = mem_resize(seq_t, A328019_C, (size_t)(L + 1) * 7);
    A328019_T = mem_resize(seq_t, A328019_T, (size_t)(L + 1));
    int start = A328019_Lmax + 1;
    for (int l = start; l <= L; l++) {
        for (int s = 0; s < 7; s++) A328019_C[l * 7 + s] = 0;
        if (l == 0) {
            A328019_C[0] = 1;
            A328019_T[0] = 1;
            continue;
        }
        for (int s = 0; s < 7; s++) {
            for (int e = 0; e < 10; e++) {
                if (e == 7) continue;
                A328019_C[l * 7 + (s * 10 + e) % 7] += A328019_C[(l - 1) * 7 + s];
            }
        }
        A328019_T[l] = A328019_T[l - 1] * 9;
    }
    A328019_Lmax = L;
}

/* count of m in [1,N] neither divisible by 7 nor containing digit 7 */
static seq_t A328019_nonF(seq_t N)
{
    int dg[1000];
    int len = 0;
    seq_t x = N;
    while (x > 0) {
        dg[len++] = (int)(x % 10);
        x /= 10;
    }
    A328019_ensure(len);
    int pw[1000];
    pw[0] = 1;
    for (int i = 1; i <= len; i++) pw[i] = (pw[i - 1] * 10) % 7;

    seq_t total = 0;
    int p = 0;
    bool alive = true;
    for (int i = len - 1; i >= 0; i--) {
        int d = dg[i];
        int L = i;
        for (int e = 0; e < d; e++) {
            if (e == 7) continue;
            int q = (p * 10 + e) % 7;
            int need = (7 - (q * pw[L]) % 7) % 7;
            total += A328019_T[L];
            total -= A328019_C[L * 7 + need];
        }
        if (d == 7) { alive = false; break; }
        p = (p * 10 + d) % 7;
    }
    if (alive && p != 0) total += 1;
    return total;
}

/* k-th positive integer divisible by 7 or containing digit 7 */
static seq_t A328019_kth(seq_t k)
{
    seq_t x = k;
    for (;;) {
        seq_t nx = k + A328019_nonF(x);
        if (nx <= x) break;   /* converged (or wrapped on overflow) */
        x = nx;
    }
    return x;
}

static seq_t *A328019_cache = NULL;
static idx_t A328019_len = 0;
static idx_t A328019_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A328019_cap) {
        idx_t nc = A328019_cap ? A328019_cap : 64;
        while (nc < n) nc *= 2;
        A328019_cache = mem_resize(seq_t, A328019_cache, nc);
        A328019_cap = nc;
    }
    if (A328019_len == 0) {
        A328019_cache[0] = 1;
        A328019_len = 1;
    }
    while (A328019_len < n) {
        A328019_cache[A328019_len] = A328019_kth(A328019_cache[A328019_len - 1]);
        A328019_len++;
    }
    return A328019_cache[n - 1];
}
