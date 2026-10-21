/*
 * A299335  Expansion of 1 / ((1 - x)^7*(1 + x)^2).
 * https://oeis.org/A299335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^6 + 54*n^5 + 575*n^4 + 3060*n^3 + 8468*n^2 + (11376
 *            if n even else 11286)*n + (5760 if n even else 5355)) / 5760 For
 *            exact integer results, use unsigned __int128 arithmetic for the
 *            numerator before truncating to seq_t. This computation works for
 *            any seq_t (including arbitrary‐precisio...
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 147, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote n to an 128‑bit unsigned integer for intermediate calculations */
    unsigned __int128 N = (unsigned __int128)n;

    /* Compute the polynomial terms */
    unsigned __int128 t6 = 2ULL * N * N * N * N * N * N;          /* 2*n^6   */
    unsigned __int128 t5 = 54ULL * N * N * N * N * N;              /* 54*n^5  */
    unsigned __int128 t4 = 575ULL * N * N * N * N;                /* 575*n^4 */
    unsigned __int128 t3 = 3060ULL * N * N * N;                  /* 3060*n^3*/
    unsigned __int128 t2 = 8468ULL * N * N;                      /* 8468*n^2*/
    unsigned __int128 t1;
    unsigned __int128 t0;

    if ((n & 1ULL) == 0) { /* even n */
        t1 = 11376ULL * N;
        t0 = 5760ULL;
    } else {              /* odd n */
        t1 = 11286ULL * N;
        t0 = 5355ULL;
    }

    /* Sum the terms and divide by 5760 */
    unsigned __int128 num = t6 + t5 + t4 + t3 + t2 + t1 + t0;
    unsigned __int128 res = num / 5760ULL; /* exact division */

    return (seq_t)res;
}
