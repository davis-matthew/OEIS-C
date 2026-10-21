/*
 * A296518  a(1) = 3, a(2) = a(5) = 1, a(3) = a(4) = a(6) = 2; a(n) =
 *          a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 6.
 * https://oeis.org/A296518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=1, a(3)=2, a(4)=2, a(5)=1, a(6)=2; For n>6:
 *            a(n)=a(n-a(n-1))+a(n-a(n-2))+a(n-a(n-3));
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A296518. */
seq_t a(idx_t n)
{
    /* Static cache for memoised values. */
    static seq_t *seq = NULL;
    static idx_t cap = 0;        /* Largest index currently stored. */

    /* Handle the base cases directly. */
    switch (n) {
    case 1: return (seq_t)3;
    case 2: return (seq_t)1;
    case 3: return (seq_t)2;
    case 4: return (seq_t)2;
    case 5: return (seq_t)1;
    case 6: return (seq_t)2;
    }

    /* n > 6 */
    if (seq == NULL) {
        /* Allocate space up to n and initialise the first six terms. */
        seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!seq) return 0;   /* In an OJ environment this should never happen. */
        seq[1] = (seq_t)3;
        seq[2] = (seq_t)1;
        seq[3] = (seq_t)2;
        seq[4] = (seq_t)2;
        seq[5] = (seq_t)1;
        seq[6] = (seq_t)2;
        cap  = 6;
    } else if (cap < n) {
        /* Grow the cache to contain index n. */
        seq = (seq_t *)realloc(seq, (size_t)(n + 1) * sizeof(seq_t));
        if (!seq) return 0;
        /* Compute new terms sequentially. */
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cap + 1; i <= n; ++i) {
        /* Recurrence needs a(i-1), a(i-2), a(i-3).  All are defined. */
        idx_t j1 = i - (idx_t)seq[i-1];
        idx_t j2 = i - (idx_t)seq[i-2];
        idx_t j3 = i - (idx_t)seq[i-3];
        seq[i] = seq[j1] + seq[j2] + seq[j3];
    }
    cap = n;
    return seq[n];
}
