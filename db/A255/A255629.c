/*
 * A255629  Number of length n+7 0..2 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1..5) are given explicitly. For n > 5 a(n) = (1/40320)*n^8 +
 *            (23/10080)*n^7 + (239/2880)*n^6 + (1541/144)*n^5 +
 *            (920887/5760)*n^4 + (442121/1440)*n^3 + (42883817/3360)*n^2 -
 *            (2307073/56)*n + 29319 The implemented code evaluates the above
 *            rational polynomial exactly by multiplying all terms by 4...
 * Range:     u8 none, u16 n <= 3, u32 n <= 5, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* index below offset, undefined, return 0 */
    /* lookup for the first five terms */
    static const uint64_t first5[] = {6561ULL, 19683ULL, 59049ULL,
                                      152106ULL, 322078ULL};
    if (n <= 5) return (seq_t)first5[n-1];

    /* for n > 5 use the empirical polynomial */
    seq_t sn = (seq_t)n;                /* convert index to seq_t */
    seq_t n2 = sn * sn;                 /* n^2 */
    seq_t n3 = n2 * sn;                 /* n^3 */
    seq_t n4 = n3 * sn;                 /* n^4 */
    seq_t n5 = n4 * sn;                 /* n^5 */
    seq_t n6 = n5 * sn;                 /* n^6 */
    seq_t n7 = n6 * sn;                 /* n^7 */
    seq_t n8 = n7 * sn;                 /* n^8 */

    /* compute numerator of the polynomial (scaled by 40320) */
    seq_t num = n8
        + ((seq_t)92   * n7)
        + ((seq_t)3346 * n6)
        + ((seq_t)431480 * n5)
        + ((seq_t)6446209 * n4)
        + ((seq_t)12379388 * n3)
        + ((seq_t)514605804 * n2)
        - ((seq_t)1661092560 * sn)
        + ((seq_t)1182142080);

    /* divide by 40320 to obtain the actual value */
    num /= (seq_t)40320;
    return num;
}
