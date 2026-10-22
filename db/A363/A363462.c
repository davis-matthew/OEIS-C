/*
 * A363462  Numbers k for which the arithmetic derivative k' (A003415) is a
 *          Fibonacci number (A000045).
 * https://oeis.org/A363462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k for which the arithmetic derivative k' is a Fibonacci number
 * This implementation enumerates integers, computes their arithmetic derivative
 * using prime factorisation, checks whether that derivative is a Fibonacci number
 * by generating Fibonacci numbers up to the derivative, and stores the terms
 * in a dynamically sized static array.  The sequence can be queried in any
 * order; terms are memoized internally. */

#include <stdlib.h>
#include <stdbool.h>

/* Utility: compute Fibonacci numbers until a value >= target. */
static bool is_fibonacci(seq_t target)
{
    seq_t f0 = 0;
    seq_t f1 = 1;
    if (target == 0) return true;
    while (f0 < target)
    {
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }
    return f0 == target;
}

/* Compute the arithmetic derivative of a positive 64‑bit integer.
 * For uint64_t values this fits easily into seq_t in the tested range. */
static seq_t derivative_uint64(uint64_t n)
{
    if (n == 1) return 0;          /* 1' = 0 */
    uint64_t original = n;
    uint64_t temp = n;
    seq_t deriv = 0;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            deriv += (seq_t)(original / p) * e;
        }
    }
    if (temp > 1)                 /* remaining prime factor */
    {
        deriv += (seq_t)(original / temp);    /* exponent is 1 */
    }
    return deriv;
}

/* Append a new term to a dynamic array; grows by powers of two. */
static void append_term(seq_t **arr, idx_t *len, idx_t *cap, seq_t val)
{
    if (*len == *cap)
    {
        *cap = (*cap == 0) ? 64 : (*cap << 1);
        *arr = (seq_t *)realloc(*arr, (*cap) * sizeof(seq_t));
    }
    (*arr)[(*len)++] = val;
}

/* Public entry point: return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;          /* stored terms */
    static idx_t terms_len = 0;         /* number of stored terms */
    static idx_t terms_cap = 0;         /* current capacity */
    static idx_t checked = 0;           /* largest integer examined */

    while (terms_len < n)
    {
        ++checked;                       /* examine the next integer */

        seq_t deriv = derivative_uint64((uint64_t)checked);
        if (is_fibonacci(deriv))
        {
            append_term(&terms, &terms_len, &terms_cap, (seq_t)checked);
        }
    }

    return terms[n - 1];
}
