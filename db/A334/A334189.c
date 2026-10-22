/*
 * A334189  Positive solutions m of the Diophantine equation x * (x+1) * (x+2)
 *          = y * (y+1) * (y+2) * (y+3) = m.
 * https://oeis.org/A334189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini, bref
 * Formula:   a(n) = x(x+1)(x+2) with the (x,y) solutions of * x(x+1)(x+2) =
 *            y(y+1)(y+2)(y+3) being (2,1), (4,2) and (55,19). * Only three
 *            terms exist: 24, 120 and 175560
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m;
    switch ((uint64_t)n) {
        case 1: { seq_t x = (seq_t)2;  m = x * (x + 1) * (x + 2); break; }
        case 2: { seq_t x = (seq_t)4;  m = x * (x + 1) * (x + 2); break; }
        case 3: { seq_t x = (seq_t)55; m = x * (x + 1) * (x + 2); break; }
        default: m = (seq_t)0;  /* no further terms are defined */
    }
    return m;
}
