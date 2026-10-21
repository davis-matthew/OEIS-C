/*
 * A265694  a(n) = n!! mod n^2 where n!! is a double factorial number
 *          (A006882).
 * https://oeis.org/A265694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!! mod n^2 (double factorial modulo n squared)
 * Range:     u8 n <= 15, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* n=0 is outside the sequence offset, but return 0 safely */
        return 0;
    }

    /* n^2 fits in seq_t for all admissible n in the test harness */
    seq_t mod = (seq_t)n * (seq_t)n;

    /* product starts as 1 mod mod (usually 1).  Using mod ensures
       correct behavior when mod == 1 (n == 1). */
    seq_t prod = 1 % mod;
    seq_t step = (seq_t)n;

    while (step > 0) {
        prod = (prod * step) % mod;

        /* Prevent unsigned underflow when step == 1 or 2 */
        if (step <= 1) {
            break;
        }
        step -= 2;
    }

    return prod;
}
