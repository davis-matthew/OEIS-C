/*
 * A322411  Compound tribonacci sequence with a(n) = A278040(A278041(n)), for
 *          n >= 0.
 * https://oeis.org/A322411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A278040(A278041(n)); with tribonacci T(0)=T(1)=0, T(2)=1,
 *            T(k)=T(k-1)+T(k-2)+T(k-3), the greedy representation m = sum of
 *            T(i), i>=3, gives A278041(m) = 3 + sum T(i+3) and A278040(m) = 1
 *            + sum T(i+2)
 * Range:     u8 n <= 11, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Evaluate 'base + sum T(i+shift)' over the greedy tribonacci representation of m (indices i>=3). */
static seq_t A322411_trib_eval(seq_t m, int shift, seq_t base)
{
    if (m == 0) return base;
    seq_t *T = mem_new(seq_t, 200);
    T[0] = 0; T[1] = 0; T[2] = 1; T[3] = 1;
    int k = 3;
    while (k < 190) {
        seq_t nx = T[k] + T[k - 1] + T[k - 2];
        T[k + 1] = nx;
        if (nx > m) break;
        k++;
    }
    /* largest index k with T[k] <= m; fill extra terms for shift */
    for (int j = k + 1; j <= k + shift; j++) {
        if (j > k + 1 || j >= 0) T[j] = T[j - 1] + T[j - 2] + T[j - 3];
    }
    seq_t sum = base;
    seq_t rem = m;
    for (int i = k; i >= 3; i--) {
        if (rem >= T[i]) {
            rem -= T[i];
            sum += T[i + shift];
        }
    }
    mem_free(T);
    return sum;
}

seq_t a(idx_t n)
{
    seq_t c = A322411_trib_eval((seq_t)n, 3, 3);   /* A278041(n) */
    return A322411_trib_eval(c, 2, 1);             /* A278040(c) */
}
