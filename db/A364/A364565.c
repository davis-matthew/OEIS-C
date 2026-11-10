/*
 * A364565  Numbers k at which point A364557 (the Möbius transform of A005941)
 *          is equal to A000010 (Euler phi function).
 * https://oeis.org/A364565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest number of the form 2^i * m with m in {1, 3,
 *            5, 85} (A364557(k) = A000010(k) forces phi(k) to be a power of
 *            2, i.e. k = 2^i * product of distinct Fermat primes; the
 *            condition pi(max odd prime)-1 = sum of log2(p-1) over the odd
 *            primes leaves only odd parts 1, 3, 5, 85). Compute...
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t v[4];
    bool inf[4];
    int j;
    v[0] = 1; v[1] = 3; v[2] = 5; v[3] = 85;
    for (j = 0; j < 4; j++) inf[j] = false;

    seq_t res = 0;
    idx_t k;
    for (k = 0; k < n; k++) {
        int best = -1;
        for (j = 0; j < 4; j++) {
            if (inf[j]) continue;
            if (best < 0 || v[j] < v[best]) best = j;
        }
        if (best < 0) return 0;
        res = v[best];
        seq_t nx = v[best] * 2;
        if (nx < v[best]) inf[best] = true;
        else v[best] = nx;
    }
    return res;
}
