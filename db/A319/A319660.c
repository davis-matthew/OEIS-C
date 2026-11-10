/*
 * A319660  2-rank of the class group of imaginary quadratic field with
 *          discriminant -k, k = A039957(n).
 * https://oeis.org/A319660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(A039957(n)) - 1, where A039957(n) is the n-th
 *            squarefree number == 3 mod 4 (found by search with trial
 *            division)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A319660_omega_sf(uint64_t m, bool *sf) {
    int cnt = 0;
    *sf = true;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (e > 1) { *sf = false; return 0; }
            cnt++;
        }
    }
    if (m > 1) cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t k = 3; ; k += 4) {
        bool sf;
        int w = A319660_omega_sf(k, &sf);
        if (sf) {
            count++;
            if (count == n) return (seq_t)(w - 1);
        }
    }
}
