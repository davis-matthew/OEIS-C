/*
 * A338730  Generating function Sum_{n >= 1} a(n)*x^n = Sum_{k>=1}
 *          k*x^(k*(3*k+1)/2)/(1-x^k).
 * https://oeis.org/A338730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1086, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A338730: a(n) = Σ k, where k*(3*k+1)/2 ≤ n and (n - k*(3*k+1)/2) mod k = 0
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 1; ; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t A = k_seq * (k_seq * 3 + 1) / 2;
        if (A > n_seq)
            break;
        if ((n_seq - A) % k_seq == 0)
            sum += k_seq;
    }

    return sum;
}
