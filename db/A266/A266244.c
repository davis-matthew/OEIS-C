/*
 * A266244  Binary representation of the n-th iteration of the "Rule 9"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10001*a(n-2) - 10000*a(n-4) for n > 7 a(0) = 1, a(1) = 0,
 *            a(2) = 101, a(3) = 1100000, a(4) = 11101, a(5) = 11110100000,
 *            a(6) = 11101, a(7) = 111111110100000. For even n > 3, a(n) =
 *            11101
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: For odd n >= 9 the recurrence is used, with the
 * first odd values computed explicitly.  The implementation
 * keeps a static cache of the odd terms to guarantee O(1)
 * retrieval for repeated calls.  64‑bit arithmetic is used
 * for small terms; for huge n the compiler's arbitrary‑precision
 * type (seq_t) handles the large integers. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the explicitly given small cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)101;
    if (n == 3)
        return (seq_t)1100000;
    if ((n % 2) == 0)            /* all even n >= 4 are 11101 */
        return (seq_t)11101;

    /* at this point n is odd and n >= 5 */
    if (n == 5)
        return (seq_t)11110100000;
    if (n == 7)
        return (seq_t)111111110100000;

    /* use recurrence for odd n >= 9 */
    static seq_t odd_cache[6000];     /* enough for n up to ~10000 */
    static size_t odd_cache_filled = 0;   /* number of terms currently cached */

    size_t k = (size_t)(n - 1) / 2;   /* map odd n to index k: n = 2*k+1 */

    if (k >= odd_cache_filled) {
        for (size_t i = odd_cache_filled; i <= k; ++i) {
            if (i == 0)
                odd_cache[0] = (seq_t)0;                          /* a(1)  */
            else if (i == 1)
                odd_cache[1] = (seq_t)1100000;                    /* a(3)  */
            else if (i == 2)
                odd_cache[2] = (seq_t)11110100000;               /* a(5)  */
            else if (i == 3)
                odd_cache[3] = (seq_t)111111110100000;            /* a(7)  */
            else {
                seq_t term1 = (seq_t)10001 * odd_cache[i-1];
                seq_t term2 = (seq_t)10000 * odd_cache[i-2];
                odd_cache[i] = term1 - term2;
            }
        }
        odd_cache_filled = k + 1;
    }
    return odd_cache[k];
}
