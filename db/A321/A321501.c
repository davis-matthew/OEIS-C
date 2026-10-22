/*
 * A321501  Numbers not of the form (x - y)(x^2 - y^2) with x > y > 0;
 *          complement of A321499.
 * https://oeis.org/A321501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, 1, 2, 4, 6, 8 for n ≤ 6; for n > 6, a(n) = 2 * (5 + 4
 *            * ((n-7)/3) + ((n-7) % 3))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the characterization that the sequence consists of
   0, 1, 8 and all even numbers not divisible by 8. */

seq_t a(idx_t n)
{
    /* First six values are fixed. */
    if (n <= 6) {
        static const seq_t first[6] = {0, 1, 2, 4, 6, 8};
        return first[n - 1];
    }

    /* For n > 6 the sequence is the even numbers not divisible by 8,
       starting after 8.  Map the index to the k-th such number. */
    idx_t m = n - 6;          /* 1‑based index among the remaining terms */
    idx_t t = m - 1;          /* zero‑based offset within that subsequence */
    idx_t b = t / 3;          /* which block of four numbers (of which 3
                                 survive the exclusion of multiples of 8)   */
    idx_t r = t % 3;          /* position within that block                 */
    idx_t k = 5 + 4 * b + r;   /* the surviving integer k (n = 2*k)          */
    return (seq_t)(k * 2);
}
