/*
 * A276385  Defined by the properties that it starts with 2, and when you
 *          successively apply DIFF, RUNS, BISECT, RUNS you get (4,1,3,1)
 *          repeated infinitely often.
 * https://oeis.org/A276385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n * t) where t is the tribonacci constant, the
 *            positive solution of x^3 = x^2 + x + 1
 * Range:     u8 n <= 90, u16 n <= 23081, u32 n <= 40000, u64 n <= 40000,
 *            u128 n <= 40000, big any n
 * Validated: standard, pass, 84 terms through n = 9927 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor(n * t) using binary search on the inequality
   k <= n*t < k+1, which is equivalent to
   f(k/n) <= 0 with f(x) = x^3 – x^2 – x – 1. */
static seq_t floor_nt(idx_t n)
{
    /* use 128‑bit arithmetic to avoid overflow for the intermediate
       cubic terms; 128‑bit is sufficient for all n that keep a(n)
       within the bounds of seq_t. */
    __int128 n128 = n;
    __int128 low  = 0;
    __int128 high = n128 * 3;       /* t < 3, so n*t < 3*n */

    while (low < high) {
        __int128 mid = (low + high + 1) >> 1;

        /* s = mid^3 – mid^2*n – mid*n^2 – n^3  */
        __int128 s = mid * mid * mid
                   - mid * mid * n128
                   - mid * n128 * n128
                   - n128 * n128 * n128;

        if (s <= 0)
            low = mid;
        else
            high = mid - 1;
    }

    return (seq_t)low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* value below the offset – undefined */

    seq_t ft = floor_nt(n);               /* floor(n * t)          */
    return (seq_t)n + ft;                 /* a(n) = n + floor(n*t) */
}
