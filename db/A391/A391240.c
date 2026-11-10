/*
 * A391240  a(n) = 1 if n occurs exactly twice in square array A391260,
 *          otherwise 0.
 * https://oeis.org/A391240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the only ordered pairs (i,j) with i*j = n and
 *            B_i(x)*B_j(x) = B_n(x) (Stern polynomials, A125184) are (1,n)
 *            and (n,1), else 0; n=1 gives 0. Computed by checking each
 *            divisor pair
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9018 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A391240_N 72

/* Stern polynomial B_m(x) via bit walk over (B_k, B_{k+1}) */
static void A391240_stern(idx_t m, seq_t *out)
{
    seq_t cur[A391240_N], nxt[A391240_N], t0[A391240_N], t1[A391240_N];
    int i;
    for (i = 0; i < A391240_N; ++i) { cur[i] = 0; nxt[i] = 0; }
    nxt[0] = 1; /* B_0 = 0, B_1 = 1 */
    int top = 63;
    while (top >= 0 && !((m >> top) & 1ULL)) --top;
    for (int b = top; b >= 0; --b) {
        int bit = (int)((m >> b) & 1ULL);
        seq_t sum[A391240_N];
        for (i = 0; i < A391240_N; ++i) sum[i] = cur[i] + nxt[i];
        if (bit == 0) {
            /* (B_{2k}, B_{2k+1}) = (x B_k, B_k + B_{k+1}) */
            t0[0] = 0;
            for (i = 1; i < A391240_N; ++i) t0[i] = cur[i - 1];
            for (i = 0; i < A391240_N; ++i) t1[i] = sum[i];
        } else {
            /* (B_{2k+1}, B_{2k+2}) = (B_k + B_{k+1}, x B_{k+1}) */
            for (i = 0; i < A391240_N; ++i) t0[i] = sum[i];
            t1[0] = 0;
            for (i = 1; i < A391240_N; ++i) t1[i] = nxt[i - 1];
        }
        for (i = 0; i < A391240_N; ++i) { cur[i] = t0[i]; nxt[i] = t1[i]; }
    }
    for (i = 0; i < A391240_N; ++i) out[i] = cur[i];
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t Bn[A391240_N], Bd[A391240_N], Be[A391240_N];
    A391240_stern(n, Bn);
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;
        A391240_stern(d, Bd);
        A391240_stern(e, Be);
        bool eq = true;
        for (int k = 0; k < A391240_N && eq; ++k) {
            seq_t s = 0;
            for (int i = 0; i <= k; ++i) s += Bd[i] * Be[k - i];
            if (s != Bn[k]) eq = false;
        }
        if (eq) return 0;
    }
    return 1;
}
