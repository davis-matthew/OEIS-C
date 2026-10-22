/*
 * A363278  Total number of parts coprime to n in the partitions of n into 3
 *          parts.
 * https://oeis.org/A363278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} Sum_{i=j..floor((n-j)/2)}
 *            ([gcd(n,n-i-j)=1] + [gcd(n,i)=1] + [gcd(n,j)=1])
 * Range:     u8 n <= 36, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t res  = 0;
    idx_t jmax = n / 3;
    for (idx_t j = 1; j <= jmax; ++j) {
        idx_t imax = (n - j) / 2;
        for (idx_t i = j; i <= imax; ++i) {
            seq_t part1 = nseq - (seq_t)i - (seq_t)j;
            seq_t part2 = (seq_t)i;
            seq_t part3 = (seq_t)j;
            if (gcd(nseq, part1) == 1) ++res;
            if (gcd(nseq, part2) == 1) ++res;
            if (gcd(nseq, part3) == 1) ++res;
        }
    }
    return res;
}
