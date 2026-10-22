/*
 * A329320  a(n) = Sum_{k=0..floor(log_2(n))} 1 - A035263(1 + floor(n/2^k)).
 * https://oeis.org/A329320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1. For n>1 let m = ⌊log₂ n⌋ (the index of the
 *            highest set bit). Write n = 2ᵐ + k, 0≤k<2ᵐ. Then a(n) = a(k mod
 *            2ᵐ⁻¹) + 1 (OEIS recurrence) The implementation reduces n by this
 *            rule until it reaches 0 or 1, counting the number of reductions.
 *            The result is that count plus 1 if the final...
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t res = 0;
    while (n >= 2) {
        idx_t m   = floor_log2_uint64(n);            // highest set bit index (>=1)
        idx_t pow2 = ((idx_t)1) << m;                 // 2^m
        idx_t k    = n - pow2;                       // remainder after removing 2^m
        idx_t mask = (((idx_t)1) << (m - 1)) - 1;     // low (m-1) bits mask
        n = k & mask;                                // k mod 2^(m-1)
        res++;                                       // each step adds 1
    }
    return res + (n == 1 ? (seq_t)1 : (seq_t)0);
}
