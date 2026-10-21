/*
 * A288029  Number of minimal edge covers in the ladder graph P_2 X P_n.
 * https://oeis.org/A288029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+2*a(n-3)+a(n-4)-a(n-6) for n>=6 with
 *            initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=17,
 *            a(5)=45. The function uses a static cache of size 10001 to
 *            support n up to about 10000 efficiently, and is valid C11 and
 *            C++17
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Ensure we have room for indices up to at least 10000. */
    enum { MAX_N = 10000 };
    static seq_t cache[MAX_N + 1] = {0};
    static idx_t max_computed = 0;

    /* Initialise base values once. */
    if (max_computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)45;
        max_computed = 5;
    }

    if (n <= max_computed)
        return cache[n];

    /* Compute terms up to n (assumes n <= MAX_N). */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)2 * cache[i - 1]          /* 2*a(i-1) */
          + cache[i - 2]                     /* + a(i-2) */
          + (seq_t)2 * cache[i - 3]          /* + 2*a(i-3) */
          + cache[i - 4]                     /* + a(i-4) */
          - cache[i - 6];                    /* - a(i-6) */
    }
    max_computed = n;
    return cache[n];
}
