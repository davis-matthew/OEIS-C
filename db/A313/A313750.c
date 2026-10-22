/*
 * A313750  Coordination sequence Gal.5.260.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A313750 – Coordination sequence Gal.5.260.4
 *  The sequence satisfies:
 *  a(0) = 1, a(1) = 5, a(2) = 10, a(3) = 15, a(4) = 20,
 *  a(5) = 26, a(6) = 32, a(7) = 36.
 *  For n ≥ 8:
 *      if n is odd:  a(n) = 5*n + 1
 *      if n is even: let k = n/2;
 *          if k is even:  a(n) = 5*n + (k-2)
 *          if k is odd:  a(n) = 5*n + k
 */

seq_t a(idx_t n)
{
    /* Handle the small exceptional indices explicitly */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)5;
        case 2: return (seq_t)10;
        case 3: return (seq_t)15;
        case 4: return (seq_t)20;
        case 5: return (seq_t)26;
        case 6: return (seq_t)32;
        case 7: return (seq_t)36;
        default: /* n >= 8 */
            break;
    }

    seq_t base = (seq_t)5 * (seq_t)n;

    if ((n & 1) != 0) {                /* n is odd */
        return base + (seq_t)1;
    } else {                           /* n is even */
        seq_t k = n >> 1;               /* k = n/2 */
        if ((k & 1) == 0) {             /* k even */
            return base + (k - (seq_t)2);
        } else {                       /* k odd */
            return base + k;
        }
    }
}
