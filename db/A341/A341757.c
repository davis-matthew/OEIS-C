/*
 * A341757  Indices of records in A341755 (= length of chain of (k,k+1)
 *          sharing a digit with k+(k+1), starting at n).
 * https://oeis.org/A341757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A341757: indices of records in A341755.
   The sequence follows a direct decimal pattern:
   * a(1) = 0.
   * For n > 1 let m = (n-1)/3.
     • If n % 3 == 1  →  a(n) = 9 followed by m '4's.
     • If n % 3 == 0  →  a(n) = 4 followed by (m-1) '8's and a terminal 6
                         (except n = 3 where a(3) = 48).
     • If n % 3 == 2  →  a(n) = 239 followed by a tail of length m-1:
                         digits alternate 3 and 9, with the last even
                         digit replaced by 6 (e.g. 2393, 23936, 239393 …).
   The digits are constructed by successive multiplication by 10
   and addition of the next digit, which works for arbitrary‑precision
   seq_t as well as for fixed‑size integer types. */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)0;
    }

    idx_t rem = n % 3;

    if (rem == 1) { /* a(3m+1) with m = (n-1)/3 >= 1 */
        idx_t m = (n - 1) / 3;
        seq_t res = (seq_t)9;
        for (idx_t i = 0; i < m; ++i)
            res = res * (seq_t)10 + (seq_t)4;
        return res;
    } else if (rem == 0) { /* a(3m) with m = n / 3 */
        idx_t m = n / 3;
        if (m == 1) {
            return (seq_t)48;          /* n = 3 */
        }
        seq_t res = (seq_t)4;
        for (idx_t i = 0; i < m - 1; ++i)
            res = res * (seq_t)10 + (seq_t)8;
        res = res * (seq_t)10 + (seq_t)6;
        return res;
    } else { /* rem == 2, a(3m+2) with m = (n-1)/3 */
        idx_t m = (n - 1) / 3;
        if (m == 0) { /* n = 2 */
            return (seq_t)9;
        }
        /* start with 239 */
        seq_t res = (seq_t)239;
        idx_t tailLen = m - 1;
        for (idx_t i = 1; i <= tailLen; ++i) {
            int digit;
            if (i & 1) {                 /* odd position */
                digit = 3;
            } else {
                if (i == tailLen)        /* last even position */
                    digit = 6;
                else
                    digit = 9;
            }
            res = res * (seq_t)10 + (seq_t)digit;
        }
        return res;
    }
}
