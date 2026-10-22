/*
 * A367690  Total number of steps of Euclid's GCD algorithm to calculate
 *          gcd(x,y) for all pairs x,y in the range 1 <= x,y <= n.
 * https://oeis.org/A367690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + n + 2 * Sum_{j=1}^{n-1} steps(n,j), a(1) = 1
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* steps(i,j) is the number of modulo iterations of Euclid's algorithm
   for ordered pair (i,j). */

#include <stdlib.h>

/* Memoized a(n) values, 1‑based. */
static seq_t *a_values = NULL;
/* Highest n that has been computed. */
static idx_t  max_n = 0;

/* Euclid's algorithm: count modulo operations until remainder is zero. */
static inline seq_t euclid_steps(idx_t a, idx_t b)
{
    seq_t steps = 0;
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
        ++steps;
    }
    return steps;
}

/* Allocate or grow the a_values array up to n. */
static void ensure_up_to(idx_t n)
{
    if (n <= max_n) return;

    /* Resize array to hold indices 0..n (0 unused). */
    seq_t *new_vals = (seq_t *)realloc(a_values, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_vals) abort();
    a_values = new_vals;

    /* Compute new entries. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t sum_steps = 0;
        for (idx_t j = 1; j < i; ++j) {
            sum_steps += euclid_steps(i, j);
        }
        seq_t val;
        if (i == 1) {
            val = 1;                                 /* a(1) = 1 */
        } else {
            val = a_values[i - 1] + (seq_t)i + sum_steps * (seq_t)2;
        }
        a_values[i] = val;
    }
    max_n = n;
}

/* Public function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; safe fallback */
    ensure_up_to(n);
    return a_values[n];
}
