/*
 * A267589  Decimal representation of the middle column of the "Rule 169"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A267589 satisfies the recurrence
   a(0)=1, a(1)=2, a(2)=5, a(3)=10, a(4)=20, a(5)=41
   a(n) = 3*a(n-1) - 2*a(n-2)   for n >= 6
   (recurrence is valid for all n >= 6; the term a(5) is an
    exceptional initial value).  No other data is required. */

seq_t a(idx_t n) {
    /* handle the first six terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)20;
    if (n == 5) return (seq_t)41;

    /* recurrence for n >= 6 */
    seq_t a_nm2 = (seq_t)20; /* a(4) */
    seq_t a_nm1 = (seq_t)41; /* a(5) */
    seq_t a_n;               /* current term */

    for (idx_t i = 6; i <= n; ++i) {
        a_n = (seq_t)3 * a_nm1 - (seq_t)2 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
