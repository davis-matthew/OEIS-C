/*
 * A339746  Positive integers of the form 2^i*3^j*k, gcd(k,6)=1, and i == j
 *          (mod 3).
 * https://oeis.org/A339746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth positive integer for which the exponents of 2 and 3 are congruent modulo 3 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 as placeholder */

    idx_t seen = 0;                 /* number of found terms so far */
    seq_t cand = 1;                 /* candidate integer to test */

    while (seen < n) {
        seq_t t = cand;
        idx_t e2 = 0, e3 = 0;

        /* exponent of 2 */
        while ((t & 1) == 0) {      /* t % 2 == 0 but using bit operation */
            e2++;
            t >>= 1;
        }
        /* exponent of 3 */
        while (t % 3 == 0) {
            e3++;
            t /= 3;
        }

        if ((e2 % 3) == (e3 % 3)) {
            seen++;
            if (seen == n)           /* found the n-th term */
                return cand;
        }
        cand += 1;
    }
    /* This line is unreachable but satisfies the compiler */
    return cand;
}
