/*
 * A258055  Concatenation of the decimal representations of the lengths
 *          (increased by 1) of the runs of zeros between successive ones in
 *          the binary representation of n.
 * https://oeis.org/A258055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Concatenate the decimal representations of (zero‑run length + 1) for each pair of successive 1 bits in the binary representation of n. */
seq_t a(idx_t n) {
    seq_t res = 0;
    int prev = -1;                  /* position of the previous 1‑bit, -1 if none seen yet */
    for (int i = 63; i >= 0; --i) { /* examine bits from most to least significant */
        if ((n >> i) & 1ULL) {      /* found a 1‑bit at position i */
            if (prev != -1) {       /* there was a previous 1‑bit, so compute the run */
                unsigned zeros = (unsigned)(prev - i - 1);   /* zeros between the two 1‑bits */
                seq_t val = (seq_t)(zeros + 1);              /* length + 1 */

                /* digit count of val (val <= 64 for 64‑bit indices) */
                int d = 0;
                seq_t t = val;
                do {
                    ++d;
                    t /= 10;
                } while (t != 0);

                /* multiply current result by 10^d and add the new part */
                seq_t mult = 1;
                for (int k = 0; k < d; ++k) mult *= 10;
                res = res * mult + val;
            }
            prev = i;               /* update previous 1‑bit position */
        }
    }
    return res; /* returns 0 if fewer than two 1‑bits are present */
}
