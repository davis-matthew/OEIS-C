/*
 * A245957  Number of length 7+3 0..n arrays with some pair in every
 *          consecutive four terms totalling exactly n.
 * https://oeis.org/A245957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 13, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245957:
   Number of length 7+3 0..n arrays with some pair in every
   consecutive four terms totalling exactly n.
   Empirical recurrence:
     a(n) = 2*a(n-1)+5*a(n-2)-12*a(n-3)-9*a(n-4)+30*a(n-5)+5*a(n-6)
            -40*a(n-7)+5*a(n-8)+30*a(n-9)-9*a(n-10)-12*a(n-11)
            +5*a(n-12)+2*a(n-13)-a(n-14)
   Initial values (a(1) .. a(14)):
     548, 31577, 300304, 2141609, 7972932, 26526337,
     67596992, 161991665, 335832580, 669329801,
     1217929488, 2155659097, 3584230244, 5837790449
   The function returns a(n) for n >= 1 (no defined behavior for n < 1). */

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t cache[20001];
    static idx_t cache_upto = 0;
    static int init = 0;

    /* initialise first 14 terms once */
    if (!init) {
        init = 1;
        cache[1] = 548ULL;
        cache[2] = 31577ULL;
        cache[3] = 300304ULL;
        cache[4] = 2141609ULL;
        cache[5] = 7972932ULL;
        cache[6] = 26526337ULL;
        cache[7] = 67596992ULL;
        cache[8] = 161991665ULL;
        cache[9] = 335832580ULL;
        cache[10] = 669329801ULL;
        cache[11] = 1217929488ULL;
        cache[12] = 2155659097ULL;
        cache[13] = 3584230244ULL;
        cache[14] = 5837790449ULL;
        cache_upto = 14;
    }

    if (n <= 14) {
        return cache[n];
    }

    /* extend cache up to n */
    if (n > cache_upto) {
        /* guard against overflow of the static array */
        if (n > 20000) {
            /* should not happen for the given limit of ~10000 */
            n = 20000;
        }
        for (idx_t i = cache_upto + 1; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;

            /* positive coefficients */
            pos += 2 * cache[i - 1];
            pos += 5 * cache[i - 2];
            pos += 30 * cache[i - 5];
            pos += 5 * cache[i - 6];
            pos += 5 * cache[i - 8];
            pos += 30 * cache[i - 9];
            pos += 5 * cache[i - 12];
            pos += 2 * cache[i - 13];

            /* negative coefficients */
            neg += 12 * cache[i - 3];
            neg += 9 * cache[i - 4];
            neg += 40 * cache[i - 7];
            neg += 9 * cache[i - 10];
            neg += 12 * cache[i - 11];
            neg += cache[i - 14];

            cache[i] = pos - neg;
        }
        cache_upto = n;
    }

    return cache[n];
}
