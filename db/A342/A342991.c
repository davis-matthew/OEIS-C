/*
 * A342991  Left(0)/right(1) turning sequence needed to traverse the
 *          Stern-Brocot tree (A007305, A047679) from the root down to e
 *          (A001113).
 * https://oeis.org/A342991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342991:
   Run‑length encoded sequence whose run lengths equal the
   continued‑fraction coefficients of e; the values alternate
   1 (left) and 0 (right), starting with a(1)=1.
   The algorithm walks the coefficient sequence until the
   cumulative length reaches the requested index. */
seq_t a(idx_t n)
{
    idx_t sum = 0;      /* cumulative length processed so far   */
    idx_t run = 0;      /* 0‑based index of the current run    */

    while (1) {
        idx_t coeff;
        if (run == 0) {
            /* the first continued‑fraction coefficient of e */
            coeff = 2;
        } else {
            /* for run ≥ 1 the coefficients are 1, except
               when (run + 1) is a multiple of 3, in which
               case the coefficient equals 2 * ((run + 1)/3).  */
            if ((run + 1) % 3 == 0) {
                coeff = 2 * ((run + 1) / 3);
            } else {
                coeff = 1;
            }
        }

        if (sum + coeff >= n) {
            /* Parity of run determines the value:
               even run → 1, odd run → 0. */
            return (seq_t)((run & 1u) ? 0 : 1);
        }

        sum += coeff;
        ++run;
    }
}
