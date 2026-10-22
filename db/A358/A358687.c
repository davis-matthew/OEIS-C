/*
 * A358687  a(n) = n! * Sum_{k=0..n} k^(3 * (n-k)) / (n-k)!.
 * https://oeis.org/A358687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(3 * (n-k)) / (n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t prod = 1;
    idx_t j = n;

    while (1) {
        seq_t base = (seq_t)(n - j);
        seq_t exp = (seq_t)(3 * j);
        seq_t pow;

        /* Handle the combinatorial convention 0^0 = 1 */
        if (base == 0 && exp == 0)
            pow = 1;
        else
            pow = big_pow(base, exp);

        sum += pow * prod;

        if (j == 0)
            break;
        j--;
        prod *= (seq_t)(j + 1);
    }

    return sum;
}
