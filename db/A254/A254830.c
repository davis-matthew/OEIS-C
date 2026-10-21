/*
 * A254830  Number of length 3 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑3 arrays with entries 1..(n+1) * such
 *            that each leading partial sum is divisible by 2, 3 or 5. *
 *            Computed by dynamic programming on residues modulo 30
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Pre‑computed list of residues modulo 30 that are divisible by 2, 3 or 5 */
static const int allowed_res[22] = {
    0, 2, 3, 4, 5, 6, 8, 9, 10, 12,
   14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 27, 28
};

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for the offset */

    /* M = n + 1 : the maximum value in the array elements */
    idx_t M = n + 1;

    /* Count of numbers in 1..M with each residue modulo 30 */
    seq_t counts[30];
    for (int r = 0; r < 30; ++r) {
        idx_t tmp = (M + ((30 - r) % 30)) / 30;   /* integer arithmetic fits in idx_t */
        counts[r] = (seq_t)tmp;
    }

    /* dp1[res] = number of ways to reach residue 'res' after the first element */
    seq_t dp1[30] = {0};
    for (int i = 0; i < 22; ++i) {
        int r = allowed_res[i];
        dp1[r] = counts[r];
    }

    /* dp2[res] = number of ways to reach residue 'res' after two elements */
    seq_t dp2[30] = {0};
    for (int i = 0; i < 22; ++i) {
        int r2 = allowed_res[i];
        seq_t sum = 0;
        for (int j = 0; j < 22; ++j) {
            int r1 = allowed_res[j];
            int diff = r2 - r1;
            if (diff < 0) diff += 30;
            sum += dp1[r1] * counts[diff];
        }
        dp2[r2] = sum;
    }

    /* dp3[res] = number of ways to reach residue 'res' after three elements */
    seq_t dp3[30] = {0};
    for (int i = 0; i < 22; ++i) {
        int r3 = allowed_res[i];
        seq_t sum = 0;
        for (int j = 0; j < 22; ++j) {
            int r2 = allowed_res[j];
            int diff = r3 - r2;
            if (diff < 0) diff += 30;
            sum += dp2[r2] * counts[diff];
        }
        dp3[r3] = sum;
    }

    /* Sum over all allowed final residues */
    seq_t total = 0;
    for (int i = 0; i < 22; ++i) {
        total += dp3[allowed_res[i]];
    }

    return total;
}
