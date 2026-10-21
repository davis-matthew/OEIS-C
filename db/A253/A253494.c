/*
 * A253494  Number of (n+1) X (7+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 49*3^(n-1) + 5322*2^(n-1) + 38777 (n>3) a(1) = 48293 a(2)
 *            = 51167 a(3) = 60586 For n<=3 the values are returned directly.
 *            For n>3 the closed‑form expression is evaluated using the
 *            provided big integer routine `big_pow`. This runs in O(log n)
 *            time
 * Range:     u8 none, u16 n <= 3, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Initial values for n = 1, 2, 3 */
    static const seq_t init[3] = {48293, 51167, 60586};

    if (n <= 3) {
        /* Offset 1: a(1) is the first term */
        return init[n - 1];
    }

    /* Compute 3^(n-1) and 2^(n-1) using the big integer routine */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 1));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 1));

    /* Apply the closed‑form formula */
    seq_t result = 49 * pow3;
    result += 5322 * pow2;
    result += 38777;

    return result;
}
