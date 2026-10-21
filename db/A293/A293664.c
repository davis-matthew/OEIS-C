/*
 * A293664  For n >= 0, let E_n be the set of exponents in expression of 2*n
 *          as a sum of distinct powers of 2 (2*n = Sum_{e in E_n} 2^e); a(n)
 *          = number of distinct values taken by the expression Sum_{e in E_n}
 *          s(e)*2^e when s runs over all functions from the positive numbers
 *          to the set { +1, -1 }.
 * https://oeis.org/A293664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct subset sums of the exponents occurring
 *            in the binary expansion of 2*n, i.e. the number of distinct
 *            values of Σ_e∈E_n s(e)*e with s(e)∈{+1,-1}
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple DP on a boolean array because
   the total sum of the exponents never exceeds 105 for n ≤ 10000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* List the exponents of the set bits in 2*n. */
    idx_t v = n << 1;                /* 2*n */
    unsigned exps[64];
    unsigned m = 0;                  /* number of exponents */
    idx_t temp = v;
    unsigned pos = 0;
    while (temp) {
        if (temp & 1ULL)
            exps[m++] = pos;
        temp >>= 1;
        pos++;
    }

    /* Compute the maximum possible sum of exponents. */
    int max_sum = 0;
    for (unsigned i = 0; i < m; ++i)
        max_sum += exps[i];

    /* DP: seen[s] is true if sum s is achievable. */
    static unsigned char seen[128];   /* 128 > 105, enough for all cases */
    for (int i = 0; i <= max_sum; ++i)
        seen[i] = 0;
    seen[0] = 1;

    for (unsigned i = 0; i < m; ++i) {
        unsigned e = exps[i];
        for (int s = max_sum; s >= (int)e; --s) {
            if (seen[s - e])
                seen[s] = 1;
        }
    }

    /* Count distinct subset sums. */
    unsigned long long count = 0;
    for (int s = 0; s <= max_sum; ++s)
        if (seen[s])
            ++count;

    return (seq_t)count;
}
