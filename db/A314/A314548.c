/*
 * A314548  Coordination sequence Gal.4.99.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; for n > 0
 *   if n mod 3 = 0 : a(n) = 5·n
 *   if n mod 3 = 1 : a(n) = 5·n – floor( (3·⌊n/3⌋) / 2 )
 *   if n mod 3 = 2 : a(n) = 5·n – floor( (3·⌊n/3⌋ + 4) / 2 )
 */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    idx_t q = n / 3;
    seq_t result = (seq_t)5 * (seq_t)n;
    int rem = n % 3;
    if (rem == 1) {
        seq_t sub = ((seq_t)3 * (seq_t)q) / 2;
        result -= sub;
    } else if (rem == 2) {
        seq_t sub = (((seq_t)3 * (seq_t)q) + 4) / 2;
        result -= sub;
    }
    return result;
}
