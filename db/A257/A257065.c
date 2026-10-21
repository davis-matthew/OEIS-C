/*
 * A257065  Number of length 3 1..(n+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of arrays (a,b,c) with 1 ≤ a,b,c ≤ n+1 such that
 *            * a, a+b, and a+b+c are each divisible by 2 or by 3. * This
 *            implementation uses residues modulo 6 to count the triples
 *            efficiently
 * Range:     u8 n <= 8, u16 n <= 59, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   resid_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/resid_allowed.h"

seq_t a(idx_t n)
{
    /* N == n+1 is the maximal element value */
    seq_t N = (seq_t)n + 1;

    /* Count of numbers in [1..N] with each residue modulo 6 */
    seq_t cnt[6];
    cnt[0] = N / 6;                            /* 6,12,18,... */
    for (int r = 1; r < 6; ++r) {
        cnt[r] = (N >= r) ? ( (N - r) / 6 + 1 ) : 0;
    }

    /* For each possible sum residue s = (a+b) mod 6, pre‑compute how many
     * values of c make (s + c) mod 6 belong to the allowed set {0,2,3,4}. */
    seq_t count_c[6] = {0};
    for (int s = 0; s < 6; ++s) {
        seq_t sum = 0;
        for (int r = 0; r < 6; ++r) {
            if (resid_allowed((s + r) % 6)) {
                sum += cnt[r];
            }
        }
        count_c[s] = sum;
    }

    /* Now accumulate the total number of triples */
    seq_t total = 0;
    /* forced residues for 'a' */
    int allowed_a[4] = {0, 2, 3, 4};
    for (int ai = 0; ai < 4; ++ai) {
        int ra = allowed_a[ai];
        seq_t ca = cnt[ra];
        if (ca == 0) continue;

        for (int rb = 0; rb < 6; ++rb) {
            int s = (ra + rb) % 6;
            if (!resid_allowed(s)) continue;
            seq_t cb = cnt[rb];
            if (cb == 0) continue;
            seq_t cc = count_c[s];
            if (cc == 0) continue;

            total += ca * cb * cc;
        }
    }

    return total;
}
