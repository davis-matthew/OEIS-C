/*
 * A385365  Consecutive states of the linear congruential pseudo-random number
 *          generator (3661*s + 30809) mod 145800 when started at s=1.
 * https://oeis.org/A385365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (3661 * a(n-1) + 30809) mod 145800, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;

    const seq_t MOD = (seq_t)145800;
    const seq_t M   = (seq_t)3661;
    const seq_t C   = (seq_t)30809;

    idx_t k = n - 1;          /* number of compositions */
    seq_t m_res = 1, c_res = 0;   /* identity transform */
    seq_t m_cur = M, c_cur = C;   /* base transform f(s) = M*s + C */

    while (k != 0) {
        if (k & 1) {                       /* result = result * cur */
            seq_t new_m = (unsigned long long)m_cur * m_res % MOD;
            seq_t new_c = ((unsigned long long)m_cur * c_res + c_cur) % MOD;
            m_res = new_m;
            c_res = new_c;
        }
        /* cur = cur * cur (squaring) */
        seq_t new_m_cur = (unsigned long long)m_cur * m_cur % MOD;
        seq_t new_c_cur = ((unsigned long long)m_cur * c_cur + c_cur) % MOD;
        m_cur = new_m_cur;
        c_cur = new_c_cur;

        k >>= 1;
    }

    seq_t ans = ((unsigned long long)m_res * (seq_t)1 + c_res) % MOD;
    return ans;
}
