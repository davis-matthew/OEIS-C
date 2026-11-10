/*
 * A274284  Numbers that are a product of distinct numbers in A006355.
 * https://oeis.org/A274284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th smallest product of a nonempty set of distinct
 *            values of A006355 (excluding 0 and 1), i.e. of 2*F(k), k>=2;
 *            computed by a bounded DFS over products with a flag sieve,
 *            doubling the bound until n terms are found
 * Range:     u8 n <= 35, u16 n <= 375, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 984 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long A274284_S[64];
static int A274284_ns;
static unsigned long long A274284_L;
static unsigned char *A274284_flag;

static void A274284_dfs(int start, unsigned long long cur) {
    for (int i = start; i < A274284_ns; i++) {
        if (A274284_S[i] > A274284_L / cur) break;
        unsigned long long p = cur * A274284_S[i];
        A274284_flag[p] = 1;
        A274284_dfs(i + 1, p);
    }
}

seq_t a(idx_t n) {
    unsigned long long L;
    for (L = 256; L <= (1ULL << 26); L *= 2) {
        A274284_L = L;
        A274284_ns = 0;
        unsigned long long x = 2, y = 4;
        A274284_S[A274284_ns++] = x;
        while (y <= L && A274284_ns < 64) {
            A274284_S[A274284_ns++] = y;
            unsigned long long z = x + y;
            x = y;
            y = z;
        }
        A274284_flag = (unsigned char *)calloc((size_t)(L + 1), 1);
        if (!A274284_flag) return 0;
        A274284_dfs(0, 1ULL);
        idx_t cnt = 0;
        for (unsigned long long v = 2; v <= L; v++) {
            if (A274284_flag[v]) {
                cnt++;
                if (cnt == n) {
                    free(A274284_flag);
                    A274284_flag = NULL;
                    return (seq_t)v;
                }
            }
        }
        free(A274284_flag);
        A274284_flag = NULL;
    }
    return 0;
}
