/*
 * A362472  E.g.f. satisfies A(x) = exp(x + x^3 * A(x)^3).
 * https://oeis.org/A362472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} (3*k+1)^(n-2*k-1) / (k! * (n‑3*k)!)
 *            (for n≥1, a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow_seq.h"

static seq_t *factorial_cache = NULL;
static size_t factorial_cap = 0;

/* Ensure factorial_cache contains factorials up to n (inclusive). */
static void ensure_factorial_size(idx_t n)
{
    if ((size_t)n < factorial_cap) return;

    size_t new_cap = factorial_cap ? factorial_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(factorial_cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);          /* out of memory */

    if (!factorial_cache) { /* first allocation */
        new_arr[0] = (seq_t)1;
        for (size_t i = 1; i < new_cap; ++i)
            new_arr[i] = new_arr[i - 1] * (seq_t)i;
    } else { /* grow from previous capacity */
        for (size_t i = factorial_cap; i < new_cap; ++i)
            new_arr[i] = new_arr[i - 1] * (seq_t)i;
    }

    factorial_cache = new_arr;
    factorial_cap   = new_cap;
}

/* OEIS sequence A362472 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    ensure_factorial_size(n);

    const seq_t f_n = factorial_cache[(size_t)n];
    seq_t sum = 0;

    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t part = f_n / factorial_cache[(size_t)k];
        part = part / factorial_cache[(size_t)(n - 3 * k)];

        seq_t base = (seq_t)(3 * k + 1);
        int exp = (int)(n - 2 * k - 1);          /* n-2k-1 ≥ 0 for n≥1 */
        seq_t power = pow_seq(base, exp);

        part = part * power;
        sum += part;
    }
    return sum;
}
