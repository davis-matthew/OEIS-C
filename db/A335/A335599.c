/*
 * A335599  Sequence is limit_{k->oo} s_k, where s_k = s_{k-1}, s_{k-1}[k-1] +
 *          2^(k-1), ..., s_{k-1}[end] + 2^(k-1) starting with s_0 = s_0[0..1]
 *          = 0,0.
 * https://oeis.org/A335599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335599:  smallest m>0 such that m+popcount(m)=n or n-1. */
#include "../utils/bitlen_seq_t.h"   /* provides unsigned bitlen_seq_t(seq_t x); */

static seq_t popcount_seq_t(seq_t x)
{
    unsigned cnt = 0;
    while (x) {
        x &= x - 1;
        ++cnt;
    }
    return (seq_t)cnt;
}

static seq_t find_min_m(idx_t target)
{
    if (target == 0)
        return (seq_t)0;
    unsigned bitlen = bitlen_seq_t((seq_t)target);
    for (int k = (int)bitlen; k >= 0; --k) {
        seq_t k_t   = (seq_t)k;
        seq_t m     = ((seq_t)target) - k_t;
        seq_t pop   = popcount_seq_t(m);
        if (pop == k_t)
            return m;
    }
    return (seq_t)0;   /* no solution (e.g., target = 1) */
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t cand1 = find_min_m(n);
    seq_t cand2 = find_min_m(n - 1);

    if (cand1 == (seq_t)0)
        return cand2;
    if (cand2 == (seq_t)0)
        return cand1;
    return (cand1 < cand2) ? cand1 : cand2;
}
