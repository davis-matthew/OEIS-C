/*
 * A345380  Number of Jacobsthal-Lucas numbers m <= n.
 * https://oeis.org/A345380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of Jacobsthal‑Lucas numbers m ≤ n *
 *            (Jacobian‑Lucas sequence: J_L(0)=2, J_L(1)=1,
 *            J_L(n)=J_L(n-1)+2*J_L(n-2). * Closed form: J_L(k)=2^k + (-1)^k,
 *            so: * even k → 2^k + 1 * odd k → 2^k - 1 * The sequence is
 *            strictly increasing. The task is to count how many of * these
 *            terms are ≤ n. F...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for uint64_t if needed */

static inline idx_t floor_log2_u64(idx_t x)
{
    /* x > 0 assumed */
    return 63 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Count odd‑index terms: J_L(odd) = 2^k - 1 ≤ n   ⇔ 2^k ≤ n+1   */
    idx_t kmax_odd;
    if (n == UINT64_MAX)
        kmax_odd = 63;                /* n+1 would overflow to 0 */
    else
        kmax_odd = floor_log2_u64(n + 1);
    seq_t odd_cnt = ((seq_t)kmax_odd + 1) / 2;  /* #odd indices ≤ kmax_odd */

    /* Count even‑index terms (only if n ≥ 2, otherwise none) */
    seq_t even_cnt = 0;
    if (n >= 2) {
        idx_t kmax_even = floor_log2_u64(n - 1);    /* 2^k ≤ n-1   */
        even_cnt = (seq_t)(kmax_even / 2) + 1;      /* #even indices ≤ kmax_even */
    }

    return odd_cnt + even_cnt;
}
