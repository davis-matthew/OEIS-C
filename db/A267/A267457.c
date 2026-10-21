/*
 * A267457  Decimal representation of the n-th iteration of the "Rule 133"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A267457
   a(n) = (3*4^n + 8*(-1)^n + (-4)^n - 24)/12   for n > 1
   Recurrence: a(n) = 17*a(n-2) - 16*a(n-4)       for n > 5
   Base: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=84, a(5)=168
*/

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n <= 5) {
        static const seq_t base[] = { (seq_t)1, (seq_t)2, (seq_t)4,
                                      (seq_t)8, (seq_t)84, (seq_t)168 };
        return base[n];
    }

    /* The recurrence uses only a(n‑2) and a(n‑4).  Keep a sliding
       buffer of the last six values to avoid any large intermediate
       computations.  All arithmetic is performed in seq_t, which
       guarantees exactness for values that fit. */
    seq_t buf[6];
    buf[0] = (seq_t)1;   /* a0 */
    buf[1] = (seq_t)2;   /* a1 */
    buf[2] = (seq_t)4;   /* a2 */
    buf[3] = (seq_t)8;   /* a3 */
    buf[4] = (seq_t)84;  /* a4 */
    buf[5] = (seq_t)168; /* a5 */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t val = (seq_t)( 17 * buf[(i - 2) % 6] - 16 * buf[(i - 4) % 6] );
        buf[i % 6] = val;
    }
    return buf[n % 6];
}
