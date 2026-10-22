/*
 * A355987  a(n) = n! * Sum_{k=1..n} 1/floor(n/k)!.
 * https://oeis.org/A355987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} 1 / floor(n/k)! (offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The function must have external linkage as requested. */
seq_t a(idx_t n)
{
    /* Precomputed factorials up to the largest n requested so far. */
    /* The array is large enough for the typical benchmark limits (n <= 10000). */
    /* For arbitrary‐precision builds this is a static array of objects, which are
       default‑constructed automatically in both C and C++. */
    enum { MAX_N = 10000 };
    static seq_t facts[MAX_N + 1];
    static idx_t cache_max = 0;              /* largest n for which facts[0..n] are valid */

    /* Ensure the factorials array is up to date. */
    if (n > cache_max) {
        /* Start from the first undefined factorial (factorial 1 if cache_max==0). */
        idx_t start = (cache_max == 0) ? 0 : cache_max;
        if (start == 0) {
            facts[0] = (seq_t)1; /* 0! = 1 */
            start = 1;
        }
        for (idx_t i = start; i <= n; ++i)
            facts[i] = facts[i - 1] * (seq_t)i;
        cache_max = n;
    }

    /* Compute the sum using the property that floor(n/k) is constant over ranges of k. */
    seq_t result = (seq_t)0;
    seq_t factorial_n = facts[n];

    idx_t k = 1;
    while (k <= n) {
        idx_t t = n / k;           /* floor(n/k) for current group */
        idx_t next_k = n / t + 1;  /* first k where the value changes */
        idx_t count = next_k - k;  /* length of the group */

        seq_t term = factorial_n / facts[t];
        result += term * (seq_t)count;

        k = next_k;
    }

    return result;
}
