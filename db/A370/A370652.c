/*
 * A370652  Number of permutations of [n] having exactly two adjacent
 *          4-cycles.
 * https://oeis.org/A370652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{k=0..⌊n/4⌋-2} (-1)^k * (n-3k-6)! / k!
 * Range:     u8 n <= 11, u16 n <= 14, u32 n <= 18, u64 n <= 26, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A370652.
 * The function works for any seq_t and idx_t.  For values that fit in seq_t,
 * it produces the exact integer result.  For larger n, the intermediate
 * values may overflow seq_t, which is unavoidable because they would
 * exceed seq_t anyway. */
seq_t a(idx_t n)
{
    /* The sequence is zero for n < 8 (first non‑zero term is a(8)=1). */
    if (n < 8)
        return 0;

    /* Term k = 0 is (n-6)!. Compute it by successive multiplication. */
    seq_t term = 1;
    for (idx_t i = 2; i <= n - 6; ++i)
        term = term * (seq_t)i;

    seq_t sum = term;                 /* alternating sum starts with k = 0 */

    /* The largest k is floor(n/4) - 2.
     * All later terms are obtained by dividing the previous term by the
     * four consecutive factors that appear in the ratio formula. */
    idx_t max_k = (n / 4) - 2;          /* n >= 8 ensures this is non‑negative */

    for (idx_t k = 1; k <= max_k; ++k)
    {
        /* Successively divide by the four small integers that appear in
         * the exact ratio: k, (n-3k-3), (n-3k-4), (n-3k-5). */
        term = term / (seq_t)k;
        term = term / (seq_t)(n - 3 * k - 3);
        term = term / (seq_t)(n - 3 * k - 4);
        term = term / (seq_t)(n - 3 * k - 5);

        if ((k & 1) == 0)
            sum += term;   /* k even -> add */
        else
            sum -= term;   /* k odd  -> subtract */
    }

    /* Finally divide by 2 as required by the formula. */
    return sum / 2;
}
