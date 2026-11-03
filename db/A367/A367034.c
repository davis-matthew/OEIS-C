/*
 * A367034  a(n) is the smallest odd number k > 1 for which the Jacobi symbol
 *          (n / k) >= 0.
 * https://oeis.org/A367034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest odd k > 1 such that (n / k) ≥ 0 (Jacobi
 *            symbol)
 * Range:     u8 n <= 11025, u16 n <= 11025, u32 n <= 11025, u64 n <= 11025,
 *            u128 n <= 11025, big any n
 * Validated: standard, pass, 121 terms through n = 9167 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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
