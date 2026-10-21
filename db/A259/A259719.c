/*
 * A259719  Number of (n+2)X(4+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 or 00000011.
 * https://oeis.org/A259719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = a(n-2) + 7*a(n-3) + 7*a(n-4) - 6*a(n-5) - 22*a(n-6) -
 *            26*a(n-7) * - 9*a(n-8) + 33*a(n-9) + 46*a(n-10) + 28*a(n-11) +
 *            10*a(n-12) * - 29*a(n-13) - 39*a(n-14) - 16*a(n-15) - 14*a(n-16)
 *            + 4*a(n-17) * + 17*a(n-18) + a(n-19) + 3*a(n-20) + 2*a(n-21) -
 *            3*a(n-22) * initial terms: * a(1)=242 a(2)=...
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 28, u64 n <= 64, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Local constant holding first 22 terms */
    static const seq_t base[22] = {
        242, 392, 1064, 2059, 2218, 5458, 12198, 16269,
        30246, 71722, 114938, 186119, 418594, 768832,
        1221808, 2484405, 4916514, 8160850, 15213700,
        30646619, 53913804, 95998916
    };

    /* Cache for computed terms up to MAX_N   */
    /* MAX_N is chosen > 10000 as per problem limits   */
    #define MAX_N 10000
    static seq_t cache[MAX_N + 1] = {0};
    static idx_t computed = 0;          /* highest n for which cache is valid */

    /* Return table entry directly when n ≤ 22   */
    if (n <= 22) return base[n - 1];

    /* Initialize cache with the first 22 terms if necessary   */
    if (computed == 0) {
        for (idx_t i = 1; i <= 22; ++i)
            cache[i] = base[i - 1];
        computed = 22;
    }

    /* Extend the cache up to the requested n   */
    if (n > computed) {
        /* Offsets and corresponding coefficients of the recurrence   */
        static const int offsets[22] = {
            2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22
        };
        static const int coeffs[22] = {
            1, 7, 7, -6, -22, -26, -9, 33, 46, 28, 10,
           -29, -39, -16, -14, 4, 17, 1, 3, 2, -3
        };

        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t pos = 0;   /* sum of positive‑coefficient terms   */
            seq_t neg = 0;   /* sum of absolute value of negative‑coefficient terms   */

            for (int k = 0; k < 22; ++k) {
                idx_t idx = i - offsets[k];
                const seq_t v = cache[idx];
                int c = coeffs[k];
                if (c > 0)
                    pos += (seq_t)c * v;
                else
                    neg += (seq_t)(-c) * v;
            }

            /* pos ≥ neg by definition of the recurrence   */
            cache[i] = pos - neg;
        }
        computed = n;
    }

    return cache[n];
}
