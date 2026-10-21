/*
 * A296239  a(n) = distance from n to nearest Fibonacci number.
 * https://oeis.org/A296239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the distance from n to the nearest Fibonacci number by enumerating
   Fibonacci numbers until exceeding n. */
seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t prev   = (seq_t)0;
    seq_t curr   = (seq_t)1;

    /* Advance until curr ≥ target */
    while (curr < target) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }

    /* Now curr ≥ target and prev ≤ target */
    seq_t diff_lower = target - prev;
    seq_t diff_upper = curr - target;

    return diff_lower <= diff_upper ? diff_lower : diff_upper;
}
