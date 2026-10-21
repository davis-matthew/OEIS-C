/*
 * A266009  Number of n X 5 integer arrays with each element equal to the
 *          number of horizontal and antidiagonal neighbors equal to itself.
 * https://oeis.org/A266009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) + 2*a(n-4) - 3*a(n-5) -
 *            a(n-6), n>12 * Initial values: a(1)=1, a(2)=4, a(3)=11, a(4)=27,
 *            a(5)=70, a(6)=182, * a(7)=483, a(8)=1281, a(9)=3389, a(10)=8986,
 *            a(11)=23818, a(12)=63151
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t max_computed = 12;  /* highest index computed */

    /* One‑time initialization */
    if (!cache) {
        cap = 13;                    /* indices 0..12 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure – unlikely in tests */

        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)27;
        cache[5] = (seq_t)70;
        cache[6] = (seq_t)182;
        cache[7] = (seq_t)483;
        cache[8] = (seq_t)1281;
        cache[9] = (seq_t)3389;
        cache[10] = (seq_t)8986;
        cache[11] = (seq_t)23818;
        cache[12] = (seq_t)63151;
    }

    /* Resize cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Return the term if it is one of the initial 12 */
    if (n <= 12) {
        return cache[(size_t)n];
    }

    /* Compute any missing terms up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)(
            2 * cache[i - 1] +      /* 2*a(n-1) */
            2 * cache[i - 2] -      /* 2*a(n-2) */
            cache[i - 3] +          /* -a(n-3) */
            2 * cache[i - 4] -      /* 2*a(n-4) */
            3 * cache[i - 5] -      /* -3*a(n-5) */
            cache[i - 6]            /* -a(n-6) */
        );
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}
