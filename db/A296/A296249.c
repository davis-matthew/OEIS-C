/*
 * A296249  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n)^2, where a(0) = 2, a(1) = 4, b(0) = 1, b(1) = 3, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n)^2, * with a(0)=2, a(1)=4, b(0)=1,
 *            b(1)=3, b(2)=5, * and a() and b() are increasing complementary
 *            sequences
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc/realloc */

seq_t a(idx_t n) {
    static seq_t *A = NULL;          /* array of computed a values   */
    static size_t cap = 0;           /* current allocation capacity */
    static size_t len = 0;           /* number of a values stored   */
    static seq_t cur = 1;            /* next unused integer (b value) */
    static size_t next_skip = 0;     /* index of next a value to skip against cur */

    if (n < len) return A[n];        /* already computed */

    while (len <= (size_t)n) {
        /* grow array if needed */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        seq_t b = cur;
        cur++;                        /* advance candidate for next b */

        /* compute a[len] */
        if (len == 0) {
            A[len] = 2;               /* a(0) */
        } else if (len == 1) {
            A[len] = 4;               /* a(1) */
        } else {
            A[len] = A[len-1] + A[len-2] + b * b;
        }

        len++;

        /* skip over cur if it coincides with any new a value */
        while (next_skip < len && A[next_skip] == cur) {
            cur++;
            next_skip++;
        }
    }
    return A[n];
}
