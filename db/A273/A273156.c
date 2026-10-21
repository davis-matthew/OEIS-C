/*
 * A273156  Product of all parts in Zeckendorf representation of n.
 * https://oeis.org/A273156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = product of all Fibonacci parts in the Zeckendorf
 *            representation of n (with Fibonacci numbers 1,2,3,5,8,…)
 * Range:     u8 n <= 30, u16 n <= 133, u32 n <= 963, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper array of Fibonacci numbers 1,2,3,5,... */
static unsigned long long *fib = NULL; /* Fibonacci values */
static size_t fib_sz = 0;              /* current number of stored values */
static size_t fib_cap = 0;             /* array capacity */

/* Ensure fib array holds at least 'need' elements. */
static void ensure_fib_size(size_t need)
{
    if (fib_cap < need) {
        size_t new_cap = fib_cap ? fib_cap << 1 : 4;
        while (new_cap < need)
            new_cap <<= 1;
        unsigned long long *tmp = (unsigned long long *)realloc(fib,
                                                               new_cap *
                                                               sizeof(unsigned long long));
        fib = tmp;
        fib_cap = new_cap;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Initialise Fibonacci table if first call. */
    if (fib_sz == 0) {
        ensure_fib_size(2);
        fib[0] = 1ULL;
        fib[1] = 2ULL;
        fib_sz = 2;
    }

    /* Expand table until last value exceeds n. */
    while (fib[fib_sz - 1] <= (unsigned long long)n) {
        unsigned long long next = fib[fib_sz - 1] + fib[fib_sz - 2];
        if (next <= (unsigned long long)n) {
            ensure_fib_size(fib_sz + 1);
            fib[fib_sz++] = next;
        } else
            break;
    }

    seq_t prod = 1;
    idx_t rem = n;

    /* Zeckendorf decomposition by subtracting largest Fibonacci <= rem. */
    while (rem > 0) {
        size_t i = fib_sz - 1;
        while (fib[i] > (unsigned long long)rem)
            --i;
        prod *= (seq_t)fib[i];
        rem -= fib[i];
    }

    return prod;
}
