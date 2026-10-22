/*
 * A363236  Number of permutations p of [n] such that each element in p has at
 *          least one neighbor with opposite parity.
 * https://oeis.org/A363236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of parity patterns with admissible runs) *
 *            (number of ways to assign odd and even numbers to these
 *            patterns) Where admissible runs are: first and last runs of
 *            length 1, interior runs of length 1 or 2, and no run of length
 *            ≥3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed factorials up to 10000 (enough for the required n). */
    static seq_t fac[10001];
    static idx_t fac_sz = 0;
    if (fac_sz == 0) {
        fac[0] = (seq_t)1;
        fac_sz = 0;
    }
    if (fac_sz < n) {
        for (idx_t i = fac_sz + 1; i <= n; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;
        fac_sz = n;
    }

    if (n == 0) return (seq_t)1;           /* empty permutation */
    if (n == 1) return (seq_t)0;           /* single element cannot satisfy the condition */

    /* Number of odd and even symbols in [n] */
    idx_t o_need = (n + 1) >> 1;           /* ceil(n/2)   */
    idx_t e_need = n >> 1;                 /* floor(n/2)  */

    seq_t fact_o = fac[o_need];
    seq_t fact_e = fac[e_need];

    idx_t T        = n - 2;                 /* total length of interior runs   */
    idx_t m_start  = (T + 1) >> 1;          /* ceil((n-2)/2)  */
    seq_t pat_cnt  = (seq_t)0;              /* count of admissible parity patterns */

    for (idx_t m = m_start; m <= T; ++m) {
        for (int s = 0; s < 2; ++s) {       /* s=0: pattern starts with odd,
                                               s=1: pattern starts with even */

            /* base = number of forced odd positions from the two boundary runs */
            idx_t base = (s == 0) ? 1 : 0;
            if (s == 0) {
                if (m & 1) base += 1;          /* runk is odd when m is odd */
            } else {
                if ((m & 1) == 0) base += 1;    /* runk is odd when m is even */
            }

            /* size of the subset of interior runs that are odd (S)   */
            idx_t s_sz = (s == 0) ? (m >> 1) : ((m + 1) >> 1);
            idx_t n_comp = m - s_sz;            /* remaining interior runs     */

            idx_t y = o_need - base;
            if (y < s_sz || y > 2 * s_sz) continue;

            idx_t y_prime = T - y;
            if (y_prime < n_comp || y_prime > 2 * n_comp) continue;

            idx_t r1 = y - s_sz;                /* number of 2's chosen for S  */
            idx_t r2 = y_prime - n_comp;        /* number of 2's for complement */

            /* binomial coefficient: choose 2's in the specified set          */
            seq_t comb1 = fac[s_sz];
            comb1 /= fac[r1];
            comb1 /= fac[s_sz - r1];

            seq_t comb2 = fac[n_comp];
            comb2 /= fac[r2];
            comb2 /= fac[n_comp - r2];

            pat_cnt += comb1 * comb2;
        }
    }

    return pat_cnt * fact_o * fact_e;
}
