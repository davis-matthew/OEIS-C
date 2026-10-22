/*
 * A309666  a(n) is the least k such that the denominators of continued
 *          fraction convergents for sqrt(k) match the first n Fibonacci
 *          numbers.
 * https://oeis.org/A309666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309666
   Formula: a(n) is given by the 16‑term linear recurrence
   a(n) = a(n-1) + 21*a(n-3) – 21*a(n-4) – 50*a(n-6) + 50*a(n-7)
          – 86*a(n-9) + 86*a(n-10) – 13*a(n-12) + 13*a(n-13)
          + a(n-15) – a(n-16),   n > 16
   with initial values
   a(1)=2,a(2)=3,a(3)=7,a(4)=7,a(5)=13,a(6)=58,a(7)=58,a(8)=135,
   a(9)=819,a(10)=819,a(11)=2081,a(12)=13834,a(13)=13834,
   a(14)=35955,a(15)=244647,a(16)=244647. */

#include <stdlib.h>
#include <stdint.h>

/* Initial values indexed from 1; index 0 is unused. */
static const seq_t init_vals[17] = {
    (seq_t)0,   /* unused */
    (seq_t)2,  (seq_t)3,  (seq_t)7,  (seq_t)7,  (seq_t)13,
    (seq_t)58, (seq_t)58, (seq_t)135, (seq_t)819, (seq_t)819,
    (seq_t)2081, (seq_t)13834, (seq_t)13834, (seq_t)35955,
    (seq_t)244647, (seq_t)244647
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* value for undefined indices */

    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cached_size = 0;          /* number of allocated slots */

    /* Remember old size before possible realloc so we can compute missing terms */
    size_t old_size = cached_size;

    /* Grow cache to include index n (we use 0..n inclusive) */
    if (cached_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;   /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failure */

        cache = tmp;
        /* Initialise the known initial values up to min(16,new_size-1) */
        for (idx_t i = 1; i <= 16 && (size_t)i < new_size; ++i)
            cache[i] = init_vals[i];

        cached_size = new_size;
    }

    /* Compute missing terms (if any) up to n */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i <= 16) continue;   /* already initialised */

        /* Recurrence using positive and negative terms to avoid underflow */
        seq_t val = cache[i-1];
        val += (seq_t)21 * cache[i-3];
        val -= (seq_t)21 * cache[i-4];
        val -= (seq_t)50 * cache[i-6];
        val += (seq_t)50 * cache[i-7];
        val -= (seq_t)86 * cache[i-9];
        val += (seq_t)86 * cache[i-10];
        val -= (seq_t)13 * cache[i-12];
        val += (seq_t)13 * cache[i-13];
        val += cache[i-15];
        val -= cache[i-16];

        cache[i] = val;
    }

    return cache[n];
}
