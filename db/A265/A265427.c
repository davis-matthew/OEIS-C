/*
 * A265427  Binary representation of the n-th iteration of the "Rule 188"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A265427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 10000*a(n-4) - 10000*a(n-6) for n>5 initial
 *            values: a(0)=1, a(1)=11, a(2)=101, a(3)=1111, a(4)=11101,
 *            a(5)=110111
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)101;
    if (n == 3) return (seq_t)1111;
    if (n == 4) return (seq_t)11101;
    if (n == 5) return (seq_t)110111;

    /* Variables to store the last six terms needed for the recurrence:
       a_minus6 = a(k-6), a_minus5 = a(k-5), … , a_minus1 = a(k-1) */
    seq_t a_minus6 = (seq_t)1;      /* a(0) */
    seq_t a_minus5 = (seq_t)11;     /* a(1) */
    seq_t a_minus4 = (seq_t)101;    /* a(2) */
    seq_t a_minus3 = (seq_t)1111;   /* a(3) */
    seq_t a_minus2 = (seq_t)11101;  /* a(4) */
    seq_t a_minus1 = (seq_t)110111; /* a(5) */

    seq_t a_curr = 0;

    /* Compute terms a(6) … a(n) using the recurrence */
    for (idx_t k = 6; k <= n; ++k) {
        a_curr = a_minus2 + (seq_t)10000 * a_minus4 - (seq_t)10000 * a_minus6;

        /* Shift the window of previous terms for the next iteration */
        a_minus6 = a_minus5;
        a_minus5 = a_minus4;
        a_minus4 = a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = a_curr;
    }

    return a_curr;
}
