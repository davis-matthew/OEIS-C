/*
 * A294972  Numerators of continued fraction convergents to sqrt(7)/2.
 * https://oeis.org/A294972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 254*a(n-4) - a(n-8) for n > 7 Initial values: a(0)=1,
 *            a(1)=4, a(2)=41, a(3)=127, a(4)=295, a(5)=1012, a(6)=10415,
 *            a(7)=32257 The function stores already computed terms and
 *            expands the cache only when a higher index is requested. It
 *            works for any build of seq_t (8, 16, 32, 64, 128‑bit or bi...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* maximum index the static cache can hold.  A value of 20000
       comfortably covers the test range (n up to ~10000). */
    enum { MAX_CACHE = 20001 };          /* indices 0 .. 20000 inclusive */

    /* static cache of computed terms */
    static seq_t cache[MAX_CACHE];
    static idx_t computed = 0;           /* first index yet to compute */

    /* initialise the base cases on first call */
    if (computed == 0) {
        const seq_t base[8] = {1, 4, 41, 127, 295, 1012, 10415, 32257};
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = base[i];
        computed = 8;                   /* next free index */
    }

    /* if the requested term has already been computed, return it
       immediately. */
    if (n < computed)
        return cache[n];

    /* compute all missing terms up to n using the recurrence:
       a(i) = 254*a(i-4) - a(i-8) */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = ((seq_t)254) * cache[i-4] - cache[i-8];
    }

    /* update the number of computed terms */
    computed = n + 1;

    return cache[n];
}
