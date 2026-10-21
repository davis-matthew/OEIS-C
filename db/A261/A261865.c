/*
 * A261865  a(n) is the least integer k such that some multiple of sqrt(k)
 *          falls strictly between n and n+1.
 * https://oeis.org/A261865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find minimal integer k such that some multiple of sqrt(k) lies strictly
 * between n and n+1.  For a fixed n we look for integers m ≥ 1 with
 *          n² < m²·k < (n+1)².
 * The inequality gives the integer interval
 *          ⌊n²/m²⌋ + 1 ≤ k ≤ ⌊((n+1)²−1)/m²⌋.
 * For each m ≤ √((n+1)²−1) we compute the smallest admissible k.
 * The least of all candidate k’s is the desired a(n). */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* square of n and (n+1)²‑1 */
    seq_t sn  = (seq_t)n;
    seq_t sn1 = sn + (seq_t)1;
    seq_t L   = sn * sn;          /* n²   */
    seq_t U   = sn1 * sn1;        /* (n+1)²  */
    U -= (seq_t)1;                /* strict upper bound */

    idx_t max_m = (idx_t)floor_sqrt(U); /* floor(sqrt(U))   */

    seq_t min_k = 0;
    bool found  = false;

    for (idx_t mi = 1; mi <= max_m; ++mi) {
        seq_t m   = (seq_t)mi;
        seq_t m2  = m * m;                 /* m² */
        seq_t k_min = L / m2 + (seq_t)1;   /* lower bound, strict > n² */
        seq_t k_max = U / m2;              /* upper bound, strict < (n+1)² */

        if (k_max >= k_min) {
            if (!found || k_min < min_k) {
                min_k = k_min;
                found = true;
                if (min_k == (seq_t)2) break; /* 2 is the global minimum */
            }
        }
    }
    return min_k;
}
