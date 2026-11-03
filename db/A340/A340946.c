/*
 * A340946  Number of ways to write n as an ordered sum of 9 squares of
 *          positive integers.
 * https://oeis.org/A340946
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 9‑tuples of positive integers whose
 *            squares sum to n
 * Range:     u8 n <= 25, u16 n <= 88, u32 n <= 1632, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 71 terms through n = 9412 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* dp after j steps counts the ordered sums of j positive squares; 9 steps of
   convolution with the squares give a(0..L). L doubles when a larger n is
   asked, so the work is O(L^1.5) overall. (The original redid the table for
   every new n and kept the squares in a fixed array of 101.) */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static idx_t L = 0;                      /* dp covers 0..L, valid if L > 0 */
    static seq_t *dp = NULL;
    if (n < 9) return (seq_t)0;
    if (n > L) {
        idx_t newL = L ? L : 1024;
        while (newL < n) newL *= 2;
        mem_free(dp);
        dp = mem_new(seq_t, newL + 1);
        seq_t *nx = mem_new(seq_t, newL + 1);
        if (!dp || !nx) abort();
        dp[0] = (seq_t)1;
        for (int step = 0; step < 9; ++step) {
            for (idx_t i = 0; i <= newL; ++i) nx[i] = (seq_t)0;
            for (idx_t s = 0; s <= newL; ++s) {
                if (dp[s] == (seq_t)0) continue;
                for (idx_t k = 1; s + k * k <= newL; ++k) nx[s + k * k] += dp[s];
            }
            seq_t *t = dp; dp = nx; nx = t;
        }
        mem_free(nx);
        L = newL;
    }
    return dp[n];
}
