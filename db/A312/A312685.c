/*
 * A312685  Coordination sequence Gal.6.253.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n>=2: a(n) = 8 + 58*⌊(n-2)/11⌋ + S[(n-2)%11]
 *            where S = [0,6,12,18,24,30,36,42,46,50,54,58]
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    /* prefix of the 11‑term difference cycle */
    static const seq_t pref[12] = {
        (seq_t)0,  (seq_t)6,  (seq_t)12, (seq_t)18,
        (seq_t)24, (seq_t)30, (seq_t)36, (seq_t)42,
        (seq_t)46, (seq_t)50, (seq_t)54, (seq_t)58
    };
    idx_t k = n - 2;
    idx_t full = k / 11;
    idx_t rem  = k % 11;
    seq_t result = (seq_t)8 + (seq_t)full * (seq_t)58 + pref[rem];
    return result;
}
