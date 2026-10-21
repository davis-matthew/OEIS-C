/*
 * A277591  Numbers k such that k/10^m == 4 mod 10, where 10^m is the greatest
 *          power of 10 that divides n.
 * https://oeis.org/A277591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 7283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277591: numbers whose rightmost non‑zero digit is 4. */
seq_t a(idx_t n)
{
    /* Count numbers that satisfy the property until we reach the n‑th. */
    idx_t count = 0;          /* how many such numbers we have seen */
    seq_t k = 1;              /* current integer being examined */

    for (;;) {
        seq_t tmp = k;

        /* Strip all trailing zeros. */
        while ((tmp % 10) == 0) {
            tmp /= 10;
        }

        /* Check whether the rightmost non‑zero digit is 4. */
        if ((tmp % 10) == 4) {
            count++;
            if (count == n) {
                return k;    /* found the n‑th term */
            }
        }

        k++;                  /* advance to the next integer */
    }
}
