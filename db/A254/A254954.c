/*
 * A254954  Number of length 7 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A254954
 * Count of length‑7 arrays with entries in 1 … (n+1) such that every
 * leading partial sum is divisible by 2, 3, 5, 7, or 11.
 *
 * This implementation uses dynamic programming over residues modulo
 * LCM(2,3,5,7,11)=2310.  For each step we add all possible array
 * entries in 1 … (n+1) (grouped by residue) and keep only states
 * whose new sum satisfies the divisibility requirement.
 *
 * The DP arrays are kept in __uint128_t to avoid overflow until the
 * final result is cast to seq_t (which may have any unsigned width).
 */

#include <stdbool.h>
#define MOD 2310

seq_t a(idx_t n) {
    const idx_t N = n + 1;                 /* maximum array entry */

    /* Count of integers in 1..N for each residue modulo MOD. */
    idx_t cnt[MOD];
    for (int d = 0; d < MOD; ++d) {
        if (d == 0) {
            cnt[d] = (N >= MOD) ? N / MOD : 0;
        } else if ((idx_t)d > N) {
            cnt[d] = 0;
        } else {
            cnt[d] = 1 + (N - d) / MOD;
        }
    }

    /* Allowed residues: divisible by 2,3,5,7 or 11. */
    bool allowed[MOD];
    for (int r = 0; r < MOD; ++r) {
        allowed[r] = ((r % 2 == 0) ||
                      (r % 3 == 0) ||
                      (r % 5 == 0) ||
                      (r % 7 == 0) ||
                      (r % 11 == 0));
    }

    /* DP arrays */
    __uint128_t cur[MOD] = {0};
    __uint128_t nxt[MOD] = {0};
    cur[0] = 1;          /* empty prefix sum */

    for (int step = 0; step < 7; ++step) {
        /* clear nxt */
        for (int i = 0; i < MOD; ++i) nxt[i] = 0;

        for (int r = 0; r < MOD; ++r) {
            __uint128_t val = cur[r];
            if (!val) continue;

            for (int d = 0; d < MOD; ++d) {
                idx_t c = cnt[d];
                if (!c) continue;

                int r2 = r + d;
                if (r2 >= MOD) r2 -= MOD;
                if (!allowed[r2]) continue;

                __uint128_t add = val * (__uint128_t)c;
                nxt[r2] += add;
            }
        }

        /* swap cur and nxt */
        for (int i = 0; i < MOD; ++i) cur[i] = nxt[i];
    }

    __uint128_t result128 = 0;
    for (int r = 0; r < MOD; ++r) result128 += cur[r];

    return (seq_t)result128;
}
