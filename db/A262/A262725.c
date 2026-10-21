/*
 * A262725  The unique function f with f(1)=1 and f(jD!+k)=(-1)^j f(k) for all
 *          D, j=1..D, and k=1..D!.
 * https://oeis.org/A262725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^A034968(n-1) (A034968 = # of 1’s in the factorial
 *            base of n‑1)
 * Range:     i8 n <= 9804, i16 n <= 9804, i32 n <= 9804, i64 n <= 9804,
 *            i128 n <= 9804, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function Computes f for OEIS sequence A262725 by iteratively applying
   the recursive definition f(j D! + k) = (-1)^j f(k).  */
seq_t a(idx_t n)
{
    if (n == 1)            /* base case */
        return (seq_t)1;

    int parity = 0;         /* 0 = even, 1 = odd number of sign flips */

    while (n > 1) {
        /* Find the largest d such that d! < n.  \"fact\" holds d! . */
        idx_t fact = 1;
        idx_t d = 1;
        while (fact * (d + 1) < n) {   /* safe: n ≤ 10 000 → d ≤ 7  */
            d++;
            fact *= d;
        }

        /* n = j*fact + rem   with 1 ≤ j ≤ d , 1 ≤ rem ≤ fact  */
        idx_t j = n / fact;
        idx_t rem = n % fact;

        if (rem == 0) {          /* adjust for the forbidden rem = 0 case */
            j--;
            rem = fact;
        }

        parity ^= (j & 1);       /* flip parity if j is odd */
        n = rem;                 /* continue with the remainder  */
    }

    return parity ? (seq_t)-1 : (seq_t)1;
}
