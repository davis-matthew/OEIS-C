/*
 * A278998  Numbers of the form (5h+1)*5^k-1 or (5h+4)*5^k-1 for h,k in N.
 * https://oeis.org/A278998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A278998: Numbers of the form (5h+1)*5^k-1 or (5h+4)*5^k-1 (h,k∈ℕ).
   Equivalently, for n≥1, a(n)=n-1 where n satisfies 
   ((n / 5^v5(n)) mod 5)∈{1,4}.  The function iterates integers n,
   checks the condition, and returns the n‑th qualifying value. */
seq_t a(idx_t n)
{
    seq_t count = 0;      /* number of qualifying terms found */
    seq_t curr  = 1;      /* current integer n to test */

    while (1) {
        /* Reduce curr by all factors of 5 */
        seq_t m = curr;
        while (m % 5 == 0) {
            m /= 5;
        }
        /* Check the remainder mod 5 of the reduced value */
        seq_t r = m % 5;
        if (r == 1 || r == 4) {
            ++count;
            if (count == n) {
                return curr - 1;
            }
        }
        ++curr;
    }
}
