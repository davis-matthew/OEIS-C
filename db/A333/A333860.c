/*
 * A333860  The maximum Hamming (binary) weight of the elements of the Collatz
 *          orbit of n, or -1 if 1 is never reached.
 * https://oeis.org/A333860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = maximum Hamming weight in the Collatz orbit of n, or -1
 *            if 1 is never reached
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the binary Hamming weight of x.
   Works for ordinary unsigned types, 128‑bit types, and arbitrary precision integer classes
   that overload &, >> and support loop termination. */
static seq_t popcount_seq_t(seq_t x) {
    seq_t count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}

/* Return the maximum Hamming weight seen while walking the Collatz orbit of n.
   All arithmetic uses the seq_t type, which is the same integer type that the
   submission system expects for the sequence value. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for 0, but protect against underflow */

    seq_t x = (seq_t)n;
    seq_t max_hw = 0;

    while (1) {
        seq_t hw = popcount_seq_t(x);
        if (hw > max_hw) max_hw = hw;

        if (x == 1) break;

        /* Collatz step: odd -> 3x + 1, even -> x/2 */
        if (x & 1) {
            x = x * 3 + 1;
        } else {
            x >>= 1;
        }
    }

    return max_hw;   /* if the loop ever failed to terminate, this function would
                        run forever; for the current range of n it does not. */
}
