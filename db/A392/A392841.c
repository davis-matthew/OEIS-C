/*
 * A392841  Number of trailing zeros in a type of Zeckendorf expansion based
 *          on the Fibonacci numbers of odd index.
 * https://oeis.org/A392841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of trailing zeros in the Zeckendorf representation
 *            of F(2k+1)-n for large k (F(2k+1) > n), i.e.
 *            A035614(F(2k+1)-n-1)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t f[200];
    seq_t j = (seq_t)n;
    int i = 1;
    f[0] = 1;
    f[1] = 2;
    /* find smallest odd-index Fibonacci (array index i odd, f[i] = F(i+2)) exceeding j */
    while (!(f[i] > j && (i & 1))) {
        f[i + 1] = f[i] + f[i - 1];
        i++;
    }
    seq_t m = f[i] - j;
    int low = 0;
    for (int t = i; t >= 0; t--) {
        if (f[t] <= m) {
            m -= f[t];
            low = t;
            if (m == 0) break;
        }
    }
    return (seq_t)low;
}
