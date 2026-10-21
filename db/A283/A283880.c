/*
 * A283880  A linear-recurrent solution to Hofstadter's Q recurrence.
 * https://oeis.org/A283880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 if n%6==0 or 4, 12 if n%6==1, 4 if n%6==3,
 *            2*F((n-2)/6+4) if n%6==2, 12*((n-5)/6)+1 if n%6==5. Here F(k) is
 *            the Fibonacci sequence with F(0)=0, F(1)=1
 * Range:     u8 n <= 49, u16 n <= 121, u32 n <= 259, u64 n <= 535,
 *            u128 n <= 1087, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached Fibonacci numbers (static, initialized on first use) */
static seq_t *fib_cache = NULL;   /* array of Fibonacci numbers */
static size_t fib_len   = 0;       /* number of elements currently computed */
static size_t fib_cap   = 0;       /* current capacity of the array */

/* Ensure that fib_cache contains Fibonacci numbers up to index `need`. */
static void ensure_fib(size_t need)
{
    if (need < fib_len) return;            /* already available */

    /* Grow capacity if necessary */
    size_t new_cap = fib_cap ? fib_cap : 2;
    while (new_cap <= need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                      /* allocation failure – leave unchanged */
    fib_cache = tmp;
    fib_cap   = new_cap;

    /* Initialise the array if it has just been created */
    if (fib_len == 0) {
        fib_cache[0] = (seq_t)0;
        fib_len = 1;
    }
    if (fib_len == 1) {
        fib_cache[1] = (seq_t)1;
        fib_len = 2;
    }

    /* Compute missing Fibonacci numbers */
    for (size_t i = fib_len; i <= need; ++i) {
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_len = need + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 – behaviour below offset irrelevant */

    idx_t r = n % 6;

    switch (r) {
    case 0:                 /* a(6k)   = 6 */
    case 4:                 /* a(6k+4) = 6 */
        return (seq_t)6;

    case 1:                 /* a(6k+1) = 12 */
        return (seq_t)12;

    case 3:                 /* a(6k+3) = 4  */
        return (seq_t)4;

    case 2: {               /* a(6k+2) = 2*F(k+4),  k = (n-2)/6 */
        idx_t k   = (n - 2) / 6;
        size_t fi = (size_t)k + 4;          /* Fibonacci index needed */
        ensure_fib(fi);
        return fib_cache[fi] * (seq_t)2;
    }

    case 5: {               /* a(6k+5) = 12*k + 1,  k = (n-5)/6 */
        idx_t k = (n - 5) / 6;
        return (seq_t)12 * (seq_t)k + (seq_t)1;
    }

    default:                /* should never happen */
        return (seq_t)0;
    }
}
