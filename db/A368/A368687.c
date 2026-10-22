/*
 * A368687  Expansion of g.f. Sum_{n>=1} q^n/(1-q^n-q^(2*n)-q^(3*n)).
 * https://oeis.org/A368687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} c_{n/d}, where c_k is the tribonacci sequence
 *            defined by c_1 = 1, c_2 = 1, c_3 = 2, c_k =
 *            c_{k-1}+c_{k-2}+c_{k-3} for k>=4
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *c = NULL;          /* tribonacci numbers */
static idx_t cap = 0;            /* current capacity of c[] */

/* Ensure that the array c has at least indices 0..n. */
static void ensure_c(idx_t n)
{
    if (n < cap)
        return;

    idx_t new_cap = (cap == 0) ? 1 : cap * 2;
    while (new_cap <= n)
        new_cap *= 2;

    seq_t *new_c = (seq_t *)calloc((size_t)new_cap, sizeof(seq_t));
    if (!new_c) {
        /* In a real system you would handle allocation failure */
        exit(EXIT_FAILURE);
    }

    /* Compute tribonacci numbers up to new_cap-1 */
    if (new_cap > 1) new_c[1] = 1;
    if (new_cap > 2) new_c[2] = 1;
    if (new_cap > 3) new_c[3] = 2;

    for (idx_t i = 4; i < new_cap; ++i)
        new_c[i] = new_c[i-1] + new_c[i-2] + new_c[i-3];

    /* Replace the old array */
    free(c);
    c = new_c;
    cap = new_cap;
}

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; behaviour for 0 is irrelevant */

    ensure_c(n);

    seq_t sum = (seq_t)0;
    /* Enumerate all divisors d of n. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t k = n / d;
            sum += c[k];            /* contribution for divisor d */
            if (d != k)
                sum += c[d];        /* contribution for divisor k */
        }
    }
    return sum;
}
