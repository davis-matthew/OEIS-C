/*
 * A347792  Beatty sequence for 2^(2/3).
 * https://oeis.org/A347792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * 2^(2/3)) = floor(cbrt(4) * n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the integer cube root of x: floor(x^(1/3)).  Uses binary search. */
static seq_t floor_cuberoot(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo <= hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = mid * mid * mid;   /* mid^3, fits for the required ranges */
        if (mid3 == x)
            return mid;
        if (mid3 < x)
            lo = mid + 1;
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;   /* hi is the largest integer with hi^3 <= x */
}

seq_t a(idx_t n)
{
    /* Convert the index to the arbitrary-precision sequence type. */
    seq_t ns = (seq_t)n;

    /* Compute n^3.  For the expected input ranges this fits within the
       available integer width (e.g., uint64_t for n ≤ 10000). */
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;

    /* Compute 4 * n^3.  This is the integer under the cube root. */
    seq_t prod = n3 * (seq_t)4;

    /* Return floor(cuberoot(prod)) = floor(n * 2^(2/3)). */
    return floor_cuberoot(prod);
}
