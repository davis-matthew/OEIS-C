/*
 * A277402  "3-Portolan numbers": number of regions formed by n-secting the
 *          angles of an equilateral triangle.
 * https://oeis.org/A277402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (30*n^2 - 45*n + 23 + 13*(-1)^n - 15*(-1)^n*n - 24*b(n))
 *            / 10 where b(n) is the 10‑periodic sequence
 *            4,0,-1,0,-1,0,-1,0,-1,0 (index 0 based). This formula is valid
 *            for all n ≥ 1
 * Range:     u8 n <= 10, u16 n <= 148, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n==0, return 0 for safety */
        return 0;

    /* Use a signed 64‑bit type for the intermediate calculation */
    long long nn = (long long)n;
    long long s  = (n % 2 == 0) ? 1LL : -1LL;   /* (-1)^n */

    /* 10‑periodic sequence b(n) */
    long long b_val;
    switch (n % 10) {
        case 0: b_val =  4; break;
        case 1: b_val =  0; break;
        case 2: b_val = -1; break;
        case 3: b_val =  0; break;
        case 4: b_val = -1; break;
        case 5: b_val =  0; break;
        case 6: b_val = -1; break;
        case 7: b_val =  0; break;
        case 8: b_val = -1; break;
        case 9: b_val =  0; break;
        default: b_val =  0;  /* unreachable */
    }

    long long numerator =
        30LL * nn * nn
      - 45LL * nn
      + 23
      + 13 * s
      - 15 * s * nn
      - 24 * b_val;

    /* The numerator is guaranteed to be divisible by 10 */
    long long result = numerator / 10;

    return (seq_t)result;
}
