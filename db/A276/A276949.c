/*
 * A276949  Index of row where n is located in array A276953 (equally: column
 *          in A276955).
 * https://oeis.org/A276949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A276949: a(n) = the smallest positive difference between a
 * non‑zero digit’s radix (k+1) and that digit’s value in the factorial
 * base representation of n.
 *
 * The factorial base representation of n is n = Σ d_k * k! where
 * 0 ≤ d_k ≤ k.  For each k with d_k > 0 we compute diff = (k+1) - d_k.
 * The answer is min diff; by definition a(0) = 0.
 *
 * This implementation precomputes factorials up to 20! (large enough
 * for all n that fit in a 64‑bit integer) and then processes the
 * representation from the most significant factorial downwards.
 */
#include <stdbool.h>

static seq_t fact[21];
static bool fact_init = false;

static void init_fact(void)
{
    if (fact_init) return;
    fact[0] = (seq_t)1;
    for (int i = 1; i <= 20; ++i)
        fact[i] = fact[i-1] * (seq_t)i;
    fact_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* by convention */
    init_fact();

    /* find largest k with k! ≤ n */
    int k = 20;
    while (k > 0 && (seq_t)n < fact[k]) --k;

    seq_t r = (seq_t)n;
    seq_t min_diff = (seq_t)0;  /* 0 indicates not set yet */

    for (int j = k; j >= 1; --j) {
        seq_t d = r / fact[j];
        if (d) {
            seq_t diff = (seq_t)(j + 1) - d;
            if (min_diff == 0 || diff < min_diff)
                min_diff = diff;
        }
        r = r % fact[j];
    }
    return min_diff;
}
