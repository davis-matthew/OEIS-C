/*
 * A256662  Sum of absolute values of terms in the minimal alternating
 *          Fibonacci representation of n.
 * https://oeis.org/A256662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f + a(f - n), where f is the smallest Fibonacci number
 *            with f ≥ n. Recurrence base: a(0) = 0, a(f) = f. This computes
 *            the sum of absolute values of terms in the minimal alternating
 *            Fibonacci representation
 * Range:     u8 n <= 144, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* fib table */
static seq_t *fib = NULL;
static size_t fib_len = 0;   /* number of Fibonacci numbers stored */
static size_t fib_cap = 0;   /* current allocated capacity */

/* ensure fib table can hold at least new_cap entries */
static void ensure_fib_capacity(size_t new_cap)
{
    if (new_cap <= fib_cap)
        return;
    /* realloc returns void* in C++, cast explicitly */
    seq_t *ptr = (seq_t*)realloc(fib, new_cap * sizeof(seq_t));
    if (!ptr)
        exit(1);               /* unrecoverable error */
    fib = ptr;
    fib_cap = new_cap;
}

/* extend fib table until it contains a value ≥ val */
static void extend_fib_to_value(seq_t val)
{
    if (fib_len == 0) {
        /* initialize with 0, 1, 1 */
        ensure_fib_capacity(3);
        fib[0] = 0;
        fib[1] = 1;
        fib[2] = 1;
        fib_len = 3;
    }
    while (fib_len > 0 && fib[fib_len - 1] < val) {
        ensure_fib_capacity(fib_len + 1);
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        fib_len++;
    }
}

/* recursive helper that implements the recurrence */
static seq_t rec_a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Ensure fib table contains a number ≥ n */
    extend_fib_to_value((seq_t)n);

    /* Find smallest index i with fib[i] ≥ n */
    size_t i = 0;
    while (fib[i] < (seq_t)n)
        i++;

    if (fib[i] == (seq_t)n)          /* exact Fibonacci number */
        return fib[i];

    /* difference d = fib[i] - n, fits into idx_t */
    idx_t d = (idx_t)(fib[i] - (seq_t)n);

    return fib[i] + rec_a(d);
}

/* Public interface required by the framework */
seq_t a(idx_t n)
{
    return rec_a(n);
}
