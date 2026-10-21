/*
 * A279743  Number of 3Xn 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -13*a(n-2) +10*a(n-3) +10*a(n-4) -30*a(n-5)
 *            +21*a(n-6) +2*a(n-7) -14*a(n-8) +10*a(n-9) -7*a(n-10) +6*a(n-11)
 *            +6*a(n-12) -2*a(n-13) -a(n-14) -2*a(n-15) -a(n-16) for n > 18
 *            Initial values: a(1)=0, a(2)=8, a(3)=35, a(4)=106, a(5)=286,
 *            a(6)=746, a(7)=1887, a(8)=4700, a(9)=11553, a(10)...
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 51, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* 1‑based array, cache[0] unused   */
    static idx_t cache_size = 0;         /* highest n cached                 */

    /* high‑precision (or widest) initial terms to avoid overflow at compile time */
    static const unsigned long long init_vals[19] = {
        0ULL,          /* a(0) unused                                     */
        0ULL, 8ULL, 35ULL, 106ULL, 286ULL, 746ULL, 1887ULL,
        4700ULL, 11553ULL, 28104ULL, 67759ULL, 162144ULL,
        385583ULL, 912098ULL, 2147806ULL, 5037496ULL,
        11773111ULL, 27427532ULL
    };

    /* coefficients for the recurrence, index 0 = coeff of a(n-1)   */
    static const int coeff[16] = {
        6,  -13, 10, 10,  -30, 21, 2,  -14,
        10,  -7, 6, 6,  -2, -1, -2,  -1
    };

    /* n should be >= 1; for safety, return 0 for n==0 */
    if (n == 0) return (seq_t)0;

    /* initialise cache with the first 18 terms if not already done */
    if (cache_size < 18) {
        size_t needed = 19;                     /* indices 0..18                    */
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        for (idx_t i = 1; i <= 18; ++i)
            cache[i] = (seq_t)init_vals[i];
        cache_size = 18;
    }

    /* if requested term already cached, return it */
    if (n <= cache_size) return cache[n];

    /* extend cache up to n */
    size_t new_len = (size_t)n + 1;              /* 1‑based, plus dummy 0           */
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    for (idx_t k = cache_size + 1; k <= n; ++k) {
        seq_t next = 0;
        for (int j = 0; j < 16; ++j) {
            idx_t idx = k - (idx_t)(j + 1);
            seq_t prod = cache[idx] * (seq_t)(coeff[j] >= 0 ? coeff[j] : -coeff[j]);
            if (coeff[j] >= 0)
                next += prod;
            else
                next -= prod;
        }
        cache[k] = next;
    }
    cache_size = n;
    return cache[n];
}
