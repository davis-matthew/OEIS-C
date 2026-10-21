/*
 * A267209  Binary representation of the middle column of the "Rule 109"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267209
   Binary representation of the middle column of Rule 109
   Recurrence: a(n) = 10*a(n-1)+a(n-3)-10*a(n-4)  for n>3
   Initial values: a(0)=1, a(1)=11, a(2)=111, a(3)=1110
*/
static const seq_t SMALL_SEQ[4] = {1, 11, 111, 1110};

seq_t a(idx_t n)
{
    /* Handle initial segment directly */
    if (n <= 3) {
        return SMALL_SEQ[n];
    }

    /* Circular buffer to hold the last five values (indices 0..4) */
    seq_t buf[5];

    /* Set the first four known terms */
    buf[0] = SMALL_SEQ[0];
    buf[1] = SMALL_SEQ[1];
    buf[2] = SMALL_SEQ[2];
    buf[3] = SMALL_SEQ[3];

    /* Iteratively apply the recurrence to reach a(n) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = buf[(i - 1) % 5] * (seq_t)10
                    + buf[(i - 3) % 5]
                    - buf[(i - 4) % 5] * (seq_t)10;
        buf[i % 5] = val;
    }

    return buf[n % 5];
}
