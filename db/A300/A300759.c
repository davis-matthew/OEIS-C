/*
 * A300759  Multiply a(n) by the first digit of a(n+1) to get a(n+2). The
 *          sequence starts with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A300759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+2) = a(n) * first_digit(a(n+1)); a(1) = 1; a(2) = 2.
 *            first_digit(x) is the most significant decimal digit of x (1–9)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 35, u64 n <= 73, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the most significant decimal digit of x (1‑9). */
static int first_digit(seq_t x)
{
    /* For any unsigned integer type this loop finishes in at most
       log10(x) iterations, which is tiny even for 128‑bit values. */
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }
    return (int)x;   /* 1 .. 9 for x > 0 */
}

/* OEIS A300759: a(1)=1, a(2)=2, a(n+2)=a(n)*first_digit(a(n+1)) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined in the sequence */

    static seq_t *cache = NULL;           /* 1‑based index: cache[1] = a(1) */
    static idx_t last = 0;                /* largest n previously computed */

    /* Grow cache if needed */
    if (n > last) {
        idx_t new_size = n + 1;           /* keep index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) abort();/* memory allocation failed */
        cache = tmp;

        /* Compute new terms up to n */
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i == 2) {
                cache[2] = (seq_t)2;
            } else {
                seq_t prev2 = cache[i - 2];
                seq_t prev1 = cache[i - 1];
                int fd = first_digit(prev1);
                cache[i] = prev2 * (seq_t)fd;
            }
        }
        last = n;
    }
    return cache[n];
}
