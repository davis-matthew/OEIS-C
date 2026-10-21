/*
 * A242505  Number of compositions of n, where the difference between the
 *          number of odd parts and the number of even parts is 7.
 * https://oeis.org/A242505
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   Number of compositions of n with (#odd parts – #even parts) = 7.
 *            For n ≥ 7, a(n) = Σ_{e=0}^{⌊(n‑7)/3⌋} [ (n‑(3e+7)) even ] ·
 *            C(2e+7, e+7) · C((n‑(3e+7))/2 + (2e+7) – 1, (2e+7) – 1)
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 36, u64 n <= 69, u128 n <= 135,
 *            big any n
 * Helpers:   nCk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/nCk.h"

seq_t a(idx_t n)
{
    if (n < 7) return 0;
    seq_t total = 0;
    for (idx_t e = 0; ; ++e) {
        idx_t o = e + 7;                /* number of odd parts */
        idx_t k = 2 * e + 7;            /* total number of parts */
        idx_t M = 3 * e + 7;            /* minimal sum: o*1 + e*2 = 3e+7 */
        if (M > n) break;
        idx_t diff = n - M;
        if ((diff & 1u) != 0u) continue; /* n-M must be even */
        idx_t R = diff >> 1;            /* remaining units of 2 after minimal */
        seq_t term = nCk(k, o) * nCk(R + k - 1, k - 1);
        total += term;
    }
    return total;
}
