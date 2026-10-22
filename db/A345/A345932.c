/*
 * A345932  a(n) = A002034(n) / gcd(n, A002034(n)), where A002034(n) gives the
 *          smallest positive integer k such that n divides k!.
 * https://oeis.org/A345932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 297, u16 n <= 9456, u32 n <= 9456, u64 n <= 9456,
 *            u128 n <= 9456, big any n
 * Helpers:   compute_A002034, gcd, min_m_for_prime_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = A002034(n) / gcd( n, A002034(n) )   */
/*  where A002034(n) is the least k with n | k! */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/compute_A002034.h"
#include "../utils/min_m_for_prime_exp.h"

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    idx_t k = compute_A002034(n);           /* A002034(n) */
    seq_t k_seq = (seq_t)k;
    seq_t n_seq = (seq_t)n;
    seq_t g = gcd(k_seq, n_seq);
    return k_seq / g;
}
