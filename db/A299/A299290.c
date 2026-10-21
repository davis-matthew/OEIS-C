/*
 * A299290  Partial sums of A299289.
 * https://oeis.org/A299290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A299290 – Partial sums of A299289
 *
 * Formula: 
 *   a(n) = (12 + 34*n + 39*n^2 + 26*n^3) / 12   for n even,
 *   a(n) = ( 9 + 34*n + 39*n^2 + 26*n^3) / 12   for n odd
 * (Both numerators are divisible by 12.)
 */
seq_t a(idx_t n) {
    /* Convert index to 64‑bit unsigned for intermediate calculations. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val;

    /* Compute the numerator using 64‑bit arithmetic (safe up to n ≈ 10^4). */
    unsigned long long n3 = nn * nn * nn;   /* n^3 */
    unsigned long long n2 = nn * nn;        /* n^2 */

    if ((nn & 1ULL) == 0) {
        /* Even n: numerator = 12 + 34*n + 39*n^2 + 26*n^3 */
        val = (12ULL + 34ULL * nn + 39ULL * n2 + 26ULL * n3) / 12ULL;
    } else {
        /* Odd n : numerator = 9 + 34*n + 39*n^2 + 26*n^3 */
        val = (9ULL  + 34ULL * nn + 39ULL * n2 + 26ULL * n3) / 12ULL;
    }

    return (seq_t)val;
}
