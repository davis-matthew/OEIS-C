/*
 * A367034  a(n) is the smallest odd number k > 1 for which the Jacobi symbol
 *          (n / k) >= 0.
 * https://oeis.org/A367034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest odd k > 1 such that (n / k) ≥ 0 (Jacobi
 *            symbol)
 * Range:     u8 n <= 9167, u16 n <= 9167, u32 n <= 9167, u64 n <= 9167,
 *            u128 n <= 9167, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t tbl[105];

    if (!ready) {
        for (idx_t i = 0; i < 105; ++i) {
            uint64_t val = (uint64_t)i;
            seq_t k = 3;
            while (1) {
                int s = jacobi_symbol(val, (uint64_t)k);
                if (s >= 0) {
                    tbl[i] = k;
                    break;
                }
                k += 2;
            }
        }
        ready = true;
    }

    idx_t idx = n % 105;
    return tbl[idx];
}
