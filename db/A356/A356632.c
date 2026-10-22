/*
 * A356632  a(n) = n! * Sum_{k=0..floor(n/2)} (n - 2*k)^k/2^k.
 * https://oeis.org/A356632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n-2k)^k / 2^k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   factorial, fast_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"  /* provides seq_t factorial(idx_t n); */
#include "../utils/fast_pow.h"

seq_t a(idx_t n) {
    /* m = floor(n/2) */
    idx_t m = n >> 1;

    /* Compute n! */
    seq_t fact_n = factorial(n);

    /* Compute 2^m and divide n! by it (exact division) */
    seq_t two_pow_m = fast_pow((seq_t)2, m);
    seq_t fact_div = fact_n / two_pow_m;

    /* Iteratively compute the sum */
    seq_t sum = 0;
    seq_t two_pow_term = two_pow_m;          /* 2^(m - k) starts at 2^m (k=0) */
    for (idx_t k = 0; k <= m; ++k) {
        seq_t base = (seq_t)(n - ((seq_t)2 * k));
        seq_t base_pow = fast_pow(base, k);      /* (n-2k)^k */
        seq_t term = base_pow * two_pow_term;    /* (n-2k)^k * 2^(m-k) */
        sum += term;
        two_pow_term /= 2;                      /* prepare 2^(m-(k+1)) */
    }

    /* Final result */
    return sum * fact_div;
}
