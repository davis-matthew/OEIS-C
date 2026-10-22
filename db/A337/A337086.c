/*
 * A337086  Number of integer-sided triangles with perimeter n where the
 *          harmonic mean of the side lengths is an integer.
 * https://oeis.org/A337086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered integer‑sided triangles with
 *            perimeter n whose harmonic mean of the side lengths is an
 *            integer
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All perimeters < 3 cannot form a triangle. */
    if (n < 3u) return 0;

    seq_t count = 0;
    /* floor(n/2) */
    idx_t half = n >> 1;           /* n/2 */

    /* a ranges from 1 to floor(n/3) */
    for (idx_t a = 1; a <= n / 3; ++a)
    {
        /* Smallest b that satisfies a+b>n/2 and b>=a:
           b_start = max(a, half+1-a)  (half = floor(n/2))          */
        idx_t b_start = half + 1 - a;
        if (b_start < a) b_start = a;

        /* largest b such that b <= c (=> b <= (n-a)/2) */
        idx_t b_end = (n - a) >> 1;   /* (n-a)/2 */
        if (b_start > b_end) continue;

        for (idx_t b = b_start; b <= b_end; ++b)
        {
            idx_t c = n - a - b;          /* c >= b by construction */
            /* Compute 3*a*b*c / (a*b + a*c + b*c) and test integrality.
               Use 128‑bit intermediates to avoid overflow. */
            __int128 A = a;
            __int128 B = b;
            __int128 C = c;

            __int128 num = 3 * A * B * C;
            __int128 den = A * B + A * C + B * C;

            if (num % den == 0)
                ++count;
        }
    }
    return count;
}
