/*
 * A315248  Coordination sequence Gal.4.123.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0: a(n) = 5*n + f(n) where f(n) is defined by
 *            f(n) = 1 if n mod 5 = 1 f(n) = 0 if n mod 5 = 2 or 3 f(n) = -1
 *            if n mod 5 = 4 f(n) = 1 if n mod 5 = 0 and n < 5 f(n) = n/5 if n
 *            mod 5 = 0 and n >= 5
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t mod = n % 5;
    seq_t base = ((seq_t)n) * (seq_t)5;

    switch (mod) {
        case 0:
            if (n < 5)
                return base + 1;
            else
                return base + (seq_t)(n / 5);
        case 1:
            return base + 1;
        case 2:
        case 3:
            return base;
        case 4:
            return base - 1;
        default:
            return base; /* unreachable */
    }
}
