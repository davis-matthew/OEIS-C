/*
 * A260692  Binary representation of the n-th iteration of the "Rule 17"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A260692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq_
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_.h"
/**
 * Formula:  a(2k)   = 1000^k
 *           a(2k+1) = (10*100^(2k+1) - 990*1000^k - 1)/9
 * (See OEIS A260692 – binary representation of the n‑th iteration of
 * Rule 17 starting with a single ON cell.)
 */

seq_t a(idx_t n)
{
    /* Even indices: a(2k) = 1000^k */
    if ((n & (idx_t)1) == (idx_t)0) {
        idx_t k = n >> (idx_t)1;
        return pow_seq_((seq_t)1000, k);
    }

    /* Odd indices: a(2k+1) = (10*100^(2k+1) - 990*1000^k - 1)/9 */
    idx_t k = (n - (idx_t)1) >> (idx_t)1;
    seq_t pow100n   = pow_seq_((seq_t)100, n);
    seq_t pow1000k  = pow_seq_((seq_t)1000, k);

    seq_t tmp = pow100n;
    tmp *= (seq_t)10;

    seq_t tmp2 = pow1000k;
    tmp2 *= (seq_t)990;

    tmp -= tmp2;
    tmp -= (seq_t)1;

    tmp /= (seq_t)9;
    return tmp;
}
