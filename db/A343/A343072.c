/*
 * A343072  Irregular triangle read by rows: T(n,k) = 2^(2n+1) * R(n,k), where
 *          R(n,k) is the k-th right endpoint after the n-th step of removal
 *          in the construction of the Smith-Volterra-Cantor set (SVC).
 * https://oeis.org/A343072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 9832, u64 n <= 9832,
 *            u128 n <= 9832, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A343072 – Irregular triangle read by rows
    a(n) = T(⌊log₂(n+1)⌋ , n – (2^⌊log₂(n+1)⌋ – 1))
    where for n≥0
    T(n,k) = 2^n + 1 + Σ_{i=0}^{n-1} b_i·4^i·(2^(n-i)+3)
    and k = Σ_{i=0}^{n-1} b_i·2^i   (b_i ∈ {0,1} are the bits of k).
    This implements the required formula directly.  */

#include <stdint.h>

/* compute T(n,k) as described above; n and k are not large (n ≤ 64)   */
static seq_t compute_T(seq_t n, seq_t k)
{
    /* n and k are small, so cast to idx_t for bit operations */
    idx_t nIdx = (idx_t)n;
    idx_t kIdx = (idx_t)k;

    seq_t res = (((seq_t)1) << (unsigned long long)nIdx) + 1;   /* 2^n + 1 */

    for (idx_t i = 0; i < nIdx; ++i)
    {
        if ((kIdx >> i) & 1ULL)
        {
            /* 4^i = 2^(2i) */
            seq_t part = (((seq_t)1) << (unsigned long long)(2*i));
            /* 2^(n-i)+3 */
            part *= (((seq_t)1) << (unsigned long long)(nIdx - i)) + 3;
            res += part;
        }
    }
    return res;
}

/* a(n) – OEIS sequence A343072 */
seq_t a(idx_t n)
{
    /* nRow = floor(log2(n+1))  (n≥0, n+1 ≥ 1) */
    idx_t t = n + 1;
    unsigned logrow = 63 - __builtin_clzll(t);   /* 0‑based log₂ */

    /* index of the first element in row nRow */
    idx_t firstIndex = ((idx_t)1 << logrow) - 1;

    idx_t kRow = n - firstIndex;   /* column within the row */

    return compute_T((seq_t)logrow, (seq_t)kRow);
}
