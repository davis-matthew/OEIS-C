/*
 * A392835  Number of ordered pairs (i,j) with 1<=i,j<=n such that
 *          gcd(i+j,i*j) = gcd(i,j).
 * https://oeis.org/A392835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(i,j) | 1≤i,j≤n and gcd(i+j,i*j)=gcd(i,j)}
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            seq_t g1 = gcd((seq_t)i, (seq_t)j);
            seq_t ij = (seq_t)i * (seq_t)j;
            seq_t g2 = gcd((seq_t)(i + j), ij);
            if (g1 == g2) {
                ++count;
            }
        }
    }
    return count;
}
