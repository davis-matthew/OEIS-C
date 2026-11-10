/*
 * A298476  Least k such that A298475(k) = n.
 * https://oeis.org/A298476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = least k with A298475(k) = n; found by a depth-first
 *            search over binary strings of length A298474(n), smallest first,
 *            pruned by the maximum partition size achievable on the remaining
 *            bits
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define A298476_MAXL 1024

static int A298476_L, A298476_N;
static char A298476_bits[A298476_MAXL];
static int A298476_f[A298476_MAXL + 1];
static int A298476_M[A298476_MAXL + 1];

static int A298476_len(int n)
{
    if (n == 1) return 1;
    if (n == 5) return 11;
    if (n % 2 == 0) return 3 * n - 4;
    return 3 * n - 3;
}

static bool A298476_pal(int i, int j)
{
    while (i < j) {
        if (A298476_bits[i] != A298476_bits[j]) return false;
        ++i; --j;
    }
    return true;
}

static bool A298476_dfs(int j)
{
    if (j == A298476_L)
        return A298476_f[j] == A298476_N;
    for (int b = (j == 0 ? 1 : 0); b <= 1; ++b) {
        A298476_bits[j] = (char)b;
        int best = A298476_MAXL;
        for (int i = 0; i <= j; ++i) {
            if (A298476_pal(i, j)) {
                int c = A298476_f[i] + 1;
                if (c < best) best = c;
            }
        }
        A298476_f[j + 1] = best;
        bool ok = true;
        for (int i = 0; i <= j + 1; ++i) {
            if (A298476_f[i] + A298476_M[A298476_L - i] < A298476_N) { ok = false; break; }
        }
        if (ok && A298476_dfs(j + 1)) return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    A298476_N = (int)n;
    A298476_L = A298476_len(A298476_N);
    for (int r = 0; r <= A298476_L; ++r) {
        int m = 0;
        while (A298476_len(m + 1) <= r) ++m;
        A298476_M[r] = m;
    }
    A298476_f[0] = 0;
    A298476_dfs(0);
    seq_t res = 0;
    for (int i = 0; i < A298476_L; ++i) {
        res = (res << 1) | (seq_t)A298476_bits[i];
    }
    return res;
}
