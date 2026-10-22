/*
 * A374044  Non-multiples of 3 whose 2-adic valuation is a multiple of 3.
 * https://oeis.org/A374044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 9899, u32 n <= 9899, u64 n <= 9899,
 *            u128 n <= 9899, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374044: Non‑multiples of 3 whose 2‑adic valuation is a multiple of 3.
   a(n) is the n‑th positive integer k such that k % 3 != 0 and
   v2(k) % 3 == 0 (where v2(k) is the exponent of 2 dividing k). */

seq_t a(idx_t n)
{
    idx_t count = 0;     /* how many valid numbers we have found */
    seq_t k = 0;         /* current candidate integer */

    while (count < n) {
        k++;                          /* next integer to test */

        /* skip multiples of 3 */
        if ((k % 3) != 0) {
            seq_t tmp = k;
            idx_t exp = 0;            /* exponent of 2 in k */

            /* count trailing zeros (v2(k)) */
            while ((tmp & 1) == 0) {
                tmp >>= 1;
                exp++;
            }

            /* accept if v2(k) is a multiple of 3 */
            if ((exp % 3) == 0) {
                count++;
            }
        }
    }

    return k;
}
