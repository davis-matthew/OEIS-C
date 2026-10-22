/*
 * A358793  Lexicographically earliest sequence of positive and unique
 *          integers such that 2*Sum_{k = 1..n} a(k) = Sum_{k = 1..n} a(a(k))
 *          for n > 1 and a(1) = 1.
 * https://oeis.org/A358793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-6)-a(n-9) for n>=16; initial values: a(1)=1,
 *            a(2)=3, a(3)=7, a(4)=5, a(5)=10, a(6)=8, a(7)=14, a(8)=16,
 *            a(9)=11, a(10)=20, a(11)=22, a(12)=13, a(13)=26, a(14)=28,
 *            a(15)=17
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms (a(1) .. a(15)) */
    static const seq_t init[15] = {
        1, 3, 7, 5, 10, 8,
        14, 16, 11, 20, 22, 13,
        26, 28, 17
    };

    /* Static storage for the generated sequence */
    static seq_t *seq = NULL;      /* array of terms */
    static idx_t cap = 0;          /* allocated capacity */
    static idx_t size = 0;         /* highest computed index */

    /* Lazy initialization */
    if (!seq) {
        /* Allocate enough space for the first 15 terms */
        cap = 15;
        seq = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        for (idx_t i = 0; i < 15; ++i) seq[i] = init[i];
        size = 15;
    }

    /* If requested term is already computed, return it */
    if (n <= size) return seq[n - 1];

    /* Ensure the array has enough capacity */
    if (n > cap) {
        idx_t newcap = cap;
        while (newcap < n) newcap *= 2;
        seq = (seq_t *)realloc(seq, (size_t)newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t k = size + 1; k <= n; ++k) {
        if (k <= 15) {
            /* Initial terms already stored */
            continue;
        }
        /* a(k) = a(k-3)+a(k-6)-a(k-9) */
        seq[k - 1] = seq[k - 4] + seq[k - 7] - seq[k - 10];
    }
    size = n;
    return seq[n - 1];
}
