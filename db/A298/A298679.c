/*
 * A298679  Start with the hexagonal tile of the Shield tiling and recursively
 *          apply the substitution rule. a(n) is the number of square tiles
 *          after n iterations.
 * https://oeis.org/A298679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 7*a(n-2) - 2*a(n-3); a(0)=0, a(1)=3, a(2)=6 */

#define A298679_MAX_N 10000
static seq_t a298679_cache[A298679_MAX_N + 1];
static idx_t a298679_cache_len = 0;

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n <= 2) {
        if (n == 0) return (seq_t)0;
        if (n == 1) return (seq_t)3;
        return (seq_t)6;
    }

    /* Ensure cache has base terms. */
    if (a298679_cache_len == 0) {
        a298679_cache[0] = (seq_t)0;
        a298679_cache[1] = (seq_t)3;
        a298679_cache[2] = (seq_t)6;
        a298679_cache_len = 3;
    }

    /* Extend cache up to n if necessary. */
    if (n + 1 > a298679_cache_len) {
        for (idx_t i = a298679_cache_len; i <= n; ++i) {
            a298679_cache[i] =
                a298679_cache[i-1] * (seq_t)2
                + a298679_cache[i-2] * (seq_t)7
                - a298679_cache[i-3] * (seq_t)2;
        }
        a298679_cache_len = n + 1;
    }

    return a298679_cache[n];
}
