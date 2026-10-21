/*
 * A281322  Number of nX4 0..2 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 58*a(n-1) -753*a(n-2) -2606*a(n-3) -532*a(n-4)
 *            +7074*a(n-5) +6399*a(n-6) -4374*a(n-7) -6561*a(n-8) for n>9.
 *            Initial values: a(1)=2, a(2)=394, a(3)=21734, a(4)=953236,
 *            a(5)=37854870, a(6)=1420883314, a(7)=51413566384,
 *            a(8)=1813061036754, a(9)=62727044483806
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n==0 – return 0 for safety */

    /* Static cache to allow calls in any order. */
    static seq_t *cache = NULL;
    static size_t len   = 0;     /* number of terms stored ( >=1 ) */
    static size_t cap   = 0;     /* allocated capacity */

    /* Grow cache if needed. */
    if ((size_t)n > cap) {
        size_t new_cap = cap == 0 ? 16 : cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = len; i < (size_t)n; ++i) {
        if (i < 9) {                      /* initial terms (0‑based indices) */
            static const seq_t first[9] = {
                2ULL,
                394ULL,
                21734ULL,
                953236ULL,
                37854870ULL,
                1420883314ULL,
                51413566384ULL,
                1813061036754ULL,
                62727044483806ULL
            };
            cache[i] = first[i];
        } else {
            seq_t term = (seq_t)58   * cache[i-1];
            term += (seq_t)7074    * cache[i-5];
            term += (seq_t)6399    * cache[i-6];
            term -= (seq_t)753     * cache[i-2];
            term -= (seq_t)2606    * cache[i-3];
            term -= (seq_t)532     * cache[i-4];
            term -= (seq_t)4374    * cache[i-7];
            term -= (seq_t)6561    * cache[i-8];
            cache[i] = term;
        }
    }
    len = (size_t)n;
    return cache[(size_t)n - 1];
}
