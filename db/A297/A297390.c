/*
 * A297390  Number of n X 3 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 neighboring 1.
 * https://oeis.org/A297390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + 2*a(n-3) - 4*a(n-4) (n≥5) Initial
 *            values (offset 1): a(1) = 3 a(2) = 9 a(3) = 19 a(4) = 57
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define A297390_MAX_N 20000   /* must be ≥ the largest n that will be asked for */

seq_t a(idx_t n)
{
    /* static cache: the array is large enough for all expected n values */
    static seq_t cache[A297390_MAX_N + 1];
    static idx_t computed = 0;   /* greatest index that has been filled */

    /* if n is still within the pre‑computed part, return it */
    if (n <= computed)
        return cache[n];

    /* initialise the first four entries once */
    if (computed < 4) {
        cache[1] = 3;
        cache[2] = 9;
        cache[3] = 19;
        cache[4] = 57;
        computed = 4;
    }

    /* extend the cache up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1]
                 + 4 * cache[i-2]
                 + 2 * cache[i-3]
                 - 4 * cache[i-4];
    }
    computed = n;
    return cache[n];
}
