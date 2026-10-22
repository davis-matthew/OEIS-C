/*
 * A370855  Numbers m such that c(0) < c(1) <= c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A370855.
   The sequence consists of all integers m such that
   c(0) < c(1) <= c(2), where c(k) is the number of times the digit k
   occurs in the base‑3 representation of m.  The function returns the
   n-th smallest such integer (1‑based index).  The value is found by
   brute‑force enumeration of successive integers and testing the
   condition above.  This approach is fast enough for the problem size
   (n up to about 10 000) and works for all supported seq_t types. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0; placeholder */

    seq_t m = 0;                   /* current integer to test   */
    seq_t result = 0;              /* holds the n-th found value */
    idx_t found = 0;               /* number of valid integers seen */

    while (found < n) {
        m++;                       /* advance to next integer   */

        unsigned cnt[3] = {0, 0, 0};
        seq_t t = m;
        while (t > 0) {            /* count base‑3 digits of m  */
            unsigned d = (unsigned)(t % 3);
            cnt[d]++;
            t /= 3;
        }

        if (cnt[0] < cnt[1] && cnt[1] <= cnt[2]) {
            found++;
            result = m;
        }
    }

    return result;
}
