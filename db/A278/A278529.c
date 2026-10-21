/*
 * A278529  a(n) = one-based position in the round in which n is removed in
 *          the Flavius sieve, 0 if n is never removed (when n is one of the
 *          terms of A000960).
 * https://oeis.org/A278529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 9732, u32 n <= 9732, u64 n <= 9732,
 *            u128 n <= 9732, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Flavius sieve: a(n) is the 1‑based position of n in the round
 * during which n is removed; 0 if n survives.  The sieve removes
 * every `step`-th element for step = 2,3,4,… until n is removed
 * or until the remaining index is less than the current step,
 * guaranteeing that n will never be removed. */
seq_t a(idx_t n)
{
    idx_t pos = n;          /* current 1‑based index of n in survivors */
    for (idx_t step = 2; step <= pos; ++step) {
        if (pos % step == 0)               /* n removed this round          */
            return (seq_t)(pos / step);    /* position within the round     */
        pos -= pos / step;                 /* update index after removals   */
    }
    return 0;     /* n survives the sieve (Flavius number) */
}
