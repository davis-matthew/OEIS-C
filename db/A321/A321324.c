/*
 * A321324  a(n) = (n^2 - c(n)) / 7 + 1 where c(n) = c(-n) = c(n+7) for all n
 *          in Z and a(n) = 1 if 0 <= n <=3 except a(1) = 0.
 * https://oeis.org/A321324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321324
   a(n) = (n^2 - c(n)) / 7 + 1,
   where c(n) is periodic modulo 7 with c = {0,8,4,9,9,4,8}
   (i.e. c(n+7)=c(n) and c(-n)=c(n)).  The special‑case a(1)=0
   is handled explicitly because the formula would otherwise
   involve a negative intermediate for unsigned seq_t. */

seq_t a(idx_t n)
{
    /* special case for n==1 */
    if (n == 1)
        return 0;

    /* periodic values of c(n) for n = 0..6 */
    static const seq_t c[7] = { 0ULL, 8ULL, 4ULL, 9ULL, 9ULL, 4ULL, 8ULL };

    /* compute n^2 (safe for all n >= 0 in seq_t) */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* index into the periodic table */
    idx_t r = n % 7;

    /* apply the formula */
    seq_t result = (sq - c[r]) / 7 + 1;

    return result;
}
