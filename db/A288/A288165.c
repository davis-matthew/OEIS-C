/*
 * A288165  Expansion of x^4/((1-x^4)*(1-x^3)*(1-x^6)*(1-x^9)).
 * https://oeis.org/A288165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_4(n/3) if n % 3 == 0, a(n) = p_4((n+8)/3) if n % 3 ==
 *            1, a(n) = p_4((n+4)/3) if n % 3 == 2, where p_4(m) = number of
 *            partitions of m into exactly 4 positive parts
 * Range:     u8 n <= 90, u16 n <= 624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t m;
    if (n % 3 == 0)           m = n / 3;
    else if (n % 3 == 1)     m = (n + 8) / 3;
    else                     m = (n + 4) / 3;   /* n % 3 == 2 */

    if (m < 4) return 0; /* fewer than 4 parts -> no partition */

    /* Count partitions of value target = m-4 using unlimited parts 1..4.
       This equals the number of partitions of m into exactly 4 parts. */
    idx_t target = m - 4;
    size_t sz = (size_t)(target + 1);
    seq_t *ways = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!ways) return 0;   /* allocation failure defaults to 0 */

    for (idx_t i = 0; i <= target; ++i) ways[i] = 0;
    ways[0] = 1;

    for (idx_t part = 1; part <= 4; ++part) {
        for (idx_t sum = part; sum <= target; ++sum) {
            ways[sum] = ways[sum] + ways[sum - part];
        }
    }

    seq_t result = ways[target];
    free(ways);
    return result;
}
