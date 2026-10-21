/*
 * A268226  Complement of A056991.
 * https://oeis.org/A268226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)+a(n-5)-a(n-6) for n>6 Initial values: a(1)=2,
 *            a(2)=3, a(3)=5, a(4)=6, a(5)=8, a(6)=11
 * Range:     u8 n <= 142, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* pre‑allocate a safe maximum for the tests (n≈10 000) */
#define MAX_CACHE 200000

static seq_t cache[MAX_CACHE + 1] = {0};
static idx_t computed = 0;

/* Computes a(n) for any n≥1, in any order.  The array 'cache' stores
   already computed values and is filled only as needed.  */
seq_t a(idx_t n)
{
    /* The problem guarantees that n will not exceed MAX_CACHE,
       but guard against out of bounds to be safe. */
    if (n > MAX_CACHE) {
        /* If n is larger, we could either abort or extend the array,
           but the specification only requires n ≈ 10 000. */
        abort();
    }

    /* If the desired term is already computed, return it. */
    if (n <= computed) {
        return cache[n];
    }

    /* Fill in the initial values up to 6 if necessary. */
    if (computed < 6) {
        if (computed < 1) cache[1] = 2;
        if (computed < 2) cache[2] = 3;
        if (computed < 3) cache[3] = 5;
        if (computed < 4) cache[4] = 6;
        if (computed < 5) cache[5] = 8;
        if (computed < 6) cache[6] = 11;
        computed = 6;
    }

    /* Compute subsequent terms using the recurrence.  Because
       a(n) may depend on a(n-5) and a(n-6), we iterate linearly
       from the first missing index up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* For i <= 6 we already have the initial values.  The
           loop guarantees i>6 here. */
        seq_t t = cache[i - 1] + cache[i - 5];
        cache[i] = t - cache[i - 6];
    }

    computed = n;
    return cache[n];
}
