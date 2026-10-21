/*
 * A271977  G_6(n), where G is the Goodstein function defined in A266201.
 * https://oeis.org/A271977
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = G_6(n) where G_1(n)=B_2(n)-1,
 *            G_{k+1}(n)=B_{k+2}(G_k(n))-1, \ * and B_k transforms the
 *            hereditary base-k representation of n by replacing each *
 *            occurrence of k with k+1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

static seq_t B_k(seq_t n, idx_t k)
{
    /* Compute B_k(n).  Works for any n (including 0). */
    seq_t result = 0;
    seq_t temp = n;
    idx_t i = 0;
    seq_t base = (seq_t)(k + 1);

    while (temp > 0) {
        idx_t digit = (idx_t)(temp % k);
        if (digit != 0) {
            /* Recursively compute B_k(i) for the exponent. */
            seq_t exp_i = B_k((seq_t)i, k);
            seq_t pow = big_pow(base, exp_i);
            result += pow * (seq_t)digit;
        }
        temp /= k;
        ++i;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* Apply the Goodstein transformation up to 6 times or until x becomes 0. */
    for (idx_t step = 1; step <= 6; ++step) {
        if (x == 0) break;
        x = B_k(x, (idx_t)(step + 1)) - 1;
    }
    return x;
}
