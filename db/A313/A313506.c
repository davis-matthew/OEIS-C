/*
 * A313506  Coordination sequence Gal.5.114.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + prefix[n] for n ≤ 10, and for n > 10 a(n) = 1 + 45 +
 *            46 * ⌊(n−10)/10⌋ + prefix2[(n−10) mod 10]. (Here d(k)=5 if k mod
 *            10 ∈ {0,1,2,5,6,9} else d(k)=4, with d(1)=4.)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Prefix sums of the first 10 differences: d(1)=4,…,d(10)=5 */
    static const seq_t pref[11] = {0, 4, 9, 13, 17, 22, 27, 31, 35, 40, 45};

    /* Prefix sums of the repeating 10‑step pattern starting at n=11
       (differences: 5,5,4,4,5,5,4,4,5,5). */
    static const seq_t pref2[11] = {0, 5, 10, 14, 18, 23, 28, 32, 36, 41, 46};

    if (n <= 10) {
        return 1 + pref[n];
    }

    idx_t rem = n - 10;
    idx_t blocks = rem / 10;
    idx_t r = rem % 10;

    return (seq_t)1 + (seq_t)45 + (seq_t)blocks * (seq_t)46 + pref2[r];
}
