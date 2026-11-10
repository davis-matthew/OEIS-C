/*
 * A397781  Number of P-partitions of n where P is the labeled poset defined
 *          by a covers b if A360010(a) > A360010(b).
 * https://oeis.org/A397781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of chains D_1 ⊇ D_2 ⊇ ... of nonempty down-sets of
 *            the layered poset (level k has k(k+1)/2 elements, A360010 gives
 *            the level of each label) with total size n; a(0)=1. Computed by
 *            a DP over down-set sizes
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    long long N = (long long)n;

    int *kk = (int *)malloc(sizeof(int) * (size_t)(N + 2));
    int *aa = (int *)malloc(sizeof(int) * (size_t)(N + 2));
    int *LL = (int *)malloc(sizeof(int) * (size_t)(N + 2)); /* size of level k+1 for state s */

    long long full = 0;
    int M = 1;
    for (int k = 0; full <= N; k++) {
        int L = (k + 1) * (k + 2) / 2;
        if (L > M) M = L;
        for (int x = 0; x < L; x++) {
            long long s = full + x;
            if (s >= 1 && s <= N) {
                kk[s] = k;
                aa[s] = x;
                LL[s] = L;
            }
        }
        full += L;
    }

    int W = M + 1;
    seq_t *C = mem_new(seq_t, (size_t)W * (size_t)W);
    for (int i = 0; i <= M; i++) {
        C[(size_t)i * W + 0] = (seq_t)1;
        for (int j = 1; j <= i; j++) {
            seq_t v = C[(size_t)(i - 1) * W + (j - 1)];
            if (j <= i - 1) v += C[(size_t)(i - 1) * W + j];
            C[(size_t)i * W + j] = v;
        }
    }

    size_t R = (size_t)N + 1;
    seq_t *F = mem_new(seq_t, R * R);
    for (long long rem = 0; rem <= N; rem++) {
        for (long long s = 1; s <= N; s++) {
            seq_t v = (rem == 0) ? (seq_t)1 : (seq_t)0;
            for (long long t = 1; t <= rem && t <= N; t++) {
                seq_t c;
                if (kk[t] < kk[s]) {
                    c = C[(size_t)LL[t] * W + aa[t]];
                } else if (kk[t] == kk[s] && aa[t] <= aa[s]) {
                    c = C[(size_t)aa[s] * W + aa[t]];
                } else continue;
                v += c * F[(size_t)t * R + (size_t)(rem - t)];
            }
            F[(size_t)s * R + (size_t)rem] = v;
        }
    }

    seq_t ans = 0;
    for (long long s = 1; s <= N; s++) {
        seq_t cnt = C[(size_t)LL[s] * W + aa[s]];
        ans += cnt * F[(size_t)s * R + (size_t)(N - s)];
    }

    mem_free(F);
    mem_free(C);
    free(kk);
    free(aa);
    free(LL);
    return ans;
}
