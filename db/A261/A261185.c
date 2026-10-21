/*
 * A261185  Flowsnake parity pattern: a(n) = (A261180(n) mod 2).
 * https://oeis.org/A261185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 iff the least significant non‑3 digit of (n‑1) in base
 *            7 is 1, 2, or 6; otherwise a(n) = 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Guard against illegal indices (offset is 1). */
    if (n == 0)
        return 0;

    idx_t m = n - 1;          /* work on (n‑1) **  */

    /* Scan the base‑7 representation from least significant digit
       until a digit that is not 3 is found. */
    while (m > 0) {
        unsigned digit = (unsigned)(m % 7);

        if (digit != 3) {                     /* non‑3 digit found */
            return (seq_t)((digit == 1 || digit == 2 || digit == 6) ? 1 : 0);
        }

        m /= 7;                               /* skip trailing 3 */
    }

    /* All digits were 3 (e.g., n = 1, 4, 10, …) → return 0. */
    return 0;
}
