/*
 * A254940  Number of length n 1..(1+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 30, u32 n <= 60, u64 n <= 118,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = number of length‑n arrays with entries 1 or 2
 *          such that every leading partial sum S_k (1 ≤ k ≤ n)
 *          is divisible by at least one of 2, 3, 5, 7 or 11.
 *   This is computed by dynamic programming over the residue
 *   of the partial sum modulo 2310 = lcm(2,3,5,7,11).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MOD 2310

seq_t a(idx_t n) {
    /* Handle the trivial case in case callers pass 0 */
    if (n == 0) return 0;

    /* Pre‑compute which residues satisfy the divisibility condition */
    static bool valid[MOD];
    static bool initialized = false;
    if (!initialized) {
        for (int s = 0; s < MOD; ++s) {
            bool ok = (s % 2 == 0) || (s % 3 == 0) ||
                      (s % 5 == 0) || (s % 7 == 0) ||
                      (s % 11 == 0);
            valid[s] = ok;
        }
        initialized = true;
    }

    /* DP arrays: cur[m] = number of sequences of current length
       having prefix sum ≡ m (mod MOD) and satisfying the condition. */
    seq_t cur[MOD] = {0};
    seq_t next[MOD] = {0};

    cur[0] = 1;  /* empty sequence, sum = 0 */

    for (idx_t step = 1; step <= n; ++step) {
        /* reset next */
        for (int i = 0; i < MOD; ++i) next[i] = 0;

        for (int mod = 0; mod < MOD; ++mod) {
            seq_t ways = cur[mod];
            if (ways == 0) continue;

            int m1 = mod + 1;
            if (m1 >= MOD) m1 -= MOD;
            if (valid[m1]) next[m1] += ways;

            int m2 = mod + 2;
            if (m2 >= MOD) m2 -= MOD;
            if (valid[m2]) next[m2] += ways;
        }

        /* copy next into cur for the next iteration */
        for (int i = 0; i < MOD; ++i) cur[i] = next[i];
    }

    /* sum over all ending residues */
    seq_t result = 0;
    for (int i = 0; i < MOD; ++i) result += cur[i];
    return result;
}
