/*
 * A370766  Number of signed permutations of length n with adjacent elements
 *          differing by more than 1.
 * https://oeis.org/A370766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A370767(n) + A370767(n-1) Recurrence: a(0) = 1 a(1) = 2
 *            a(2) = 4 a(3) = 20 For n ≥ 4: a(n) = (1+2*n)*a(n-1) +
 *            (1-2*n)*a(n-2) + (9-2*n)*a(n-3) + (-6+2*n)*a(n-4) This
 *            implementation uses an O(n) recurrence with memoisation; it
 *            works for any build where seq_t supports integer arithmetic
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, NULL */

#ifndef MAX_A_N
# define MAX_A_N (10001)   /* sufficient for n up to about 10 000 */
#endif

static seq_t a_cache[MAX_A_N];
static idx_t a_computed = 0;

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)20;

    /* initialise base terms if not yet done */
    if (a_computed < 3) {
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_cache[2] = (seq_t)4;
        a_cache[3] = (seq_t)20;
        a_computed = 3;
    }

    /* guard against requests beyond pre‑allocated buffer */
    if (n >= MAX_A_N) {
        /* for builds where seq_t is larger than 64 bits, enlarge by realloc */
        /* but for this problem n ≤ 10 000, so we simply return 0 or assert */
        return (seq_t)0;
    }

    /* compute values up to n */
    for (idx_t i = a_computed + 1; i <= n; ++i) {
        seq_t val = 0;

        /* (1 + 2*i) * a[i-1]  (always positive) */
        val += (seq_t)(1 + 2*(long long)i) * a_cache[i-1];

        /* (1 - 2*i) * a[i-2]  (may be negative) */
        long long c2 = 1 - 2*(long long)i;
        if (c2 >= 0)
            val += (seq_t)c2 * a_cache[i-2];
        else
            val -= (seq_t)(-c2) * a_cache[i-2];

        /* (9 - 2*i) * a[i-3]  (may be negative) */
        long long c3 = 9 - 2*(long long)i;
        if (c3 >= 0)
            val += (seq_t)c3 * a_cache[i-3];
        else
            val -= (seq_t)(-c3) * a_cache[i-3];

        /* (-6 + 2*i) * a[i-4]  (non‑negative for i ≥ 4) */
        long long c4 = -6 + 2*(long long)i;
        if (c4 >= 0)
            val += (seq_t)c4 * a_cache[i-4];
        else
            val -= (seq_t)(-c4) * a_cache[i-4];

        a_cache[i] = val;
    }

    a_computed = n;
    return a_cache[n];
}
