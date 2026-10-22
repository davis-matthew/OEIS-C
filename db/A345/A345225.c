/*
 * A345225  a(n) = order of 2-primary subgroup of the group K_n(Z).
 * https://oeis.org/A345225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A345225: 2‑primary subgroup orders of K_n(Z)
   a(n) =
       1  if n mod 8 = 0,4,5,6
       2  if n mod 8 = 1,2
       16 if n mod 8 = 3
       2*(n+1) if n mod 8 = 7
*/

seq_t a(idx_t n)
{
    /* compute n modulo 8 */
    idx_t m = n & 7;   /* same as n % 8 but faster */

    switch (m) {
        case 0: case 4: case 5: case 6:
            return (seq_t)1;

        case 1: case 2:
            return (seq_t)2;

        case 3:
            return (seq_t)16;

        case 7: {
            /* cast to seq_t before arithmetic to avoid idx_t overflow */
            seq_t nseq = (seq_t)n;
            return (seq_t)2 * (nseq + (seq_t)1);
        }

        /* the switch covers all values 0..7, so the code should never reach here */
    }
    return (seq_t)0;   /* fallback, should never be used */
}
