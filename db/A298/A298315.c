/*
 * A298315  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +4*a(n-2) +12*a(n-3) -15*a(n-4) -40*a(n-5)
 *            -53*a(n-6) +62*a(n-7) +133*a(n-8) +92*a(n-9) -92*a(n-10)
 *            -146*a(n-11) -11*a(n-12) +102*a(n-13) +49*a(n-14) -49*a(n-15)
 *            -64*a(n-16) -48*a(n-17) for n > 18 a(1) = 3, a(2) = 3, a(3) = 3,
 *            a(4) = 9, a(5) = 17, a(6) = 48, a(7) = 94, a(8) = 234, a...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const long long coeff[17] = {
    1LL, 4LL, 12LL, -15LL, -40LL, -53LL, 62LL, 133LL, 92LL,
   -92LL, -146LL, -11LL, 102LL, 49LL, -49LL, -64LL, -48LL
};

static const seq_t init_val[18] = {
    3ULL, 3ULL, 3ULL, 9ULL, 17ULL, 48ULL, 94ULL, 234ULL, 589ULL,
    1333ULL, 3352ULL, 8249ULL, 19791ULL, 49537ULL, 121580ULL,
    298541ULL, 742378ULL, 1829015ULL
};

seq_t a(idx_t n) {
    /* Use a static cache large enough for typical test sizes (≈10⁴). */
    static seq_t cache[20005];
    static idx_t max_computed = 0;

    /* Initialise the cache on first call. */
    if (max_computed == 0) {
        for (idx_t i = 0; i < 18; ++i)
            cache[i] = init_val[i];
        max_computed = 18;
    }

    /* Return cached value if already computed. */
    if (n <= max_computed)
        return cache[n - 1];

    /* Compute values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 17; ++k) {
            seq_t prod = cache[i - 2 - k] * (seq_t)(coeff[k] >= 0 ? coeff[k] : -coeff[k]);
            if (coeff[k] >= 0)
                pos += prod;
            else
                neg += prod;
        }
        /* The recurrence guarantees pos >= neg, so subtraction is safe. */
        cache[i - 1] = pos - neg;
    }
    max_computed = n;
    return cache[n - 1];
}
