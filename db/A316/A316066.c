/*
 * A316066  Coordination sequence Gal.5.329.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A316066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n < 24 the coordination sequence is given by the following
 *            initial values: a(0)=1, a(1)=6, a(2)=9, a(3)=16, a(4)=22,
 *            a(5)=24, a(6)=33, a(7)=37, a(8)=41, a(9)=49, a(10)=54, a(11)=56,
 *            a(12)=67, a(13)=68, a(14)=73, a(15)=83, a(16)=85, a(17)=87,
 *            a(18)=101, a(19)=99, a(20)=104, a(21)=117, a(22)=11...
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial 24 terms */
    static const seq_t init[24] =
    {
        1, 6, 9, 16, 22, 24,
        33, 37, 41, 49, 54, 56,
        67, 68, 73, 83, 85, 87,
        101, 99, 104, 117, 116, 118
    };

    if (n < 24)
        return init[n];

    /* for n ≥ 24 compute using the recurrence
       a(6k+r) = base_r + (k – threshold_r) * delta_r */
    idx_t k = n / 6;
    idx_t r = n % 6;

    /* threshold index where the constant differences become stable */
    static const idx_t threshold[6] = { 2, 1, 3, 2, 2, 2 };
    /* value at the first index after the threshold */
    static const seq_t base[6] = { 67, 37, 104, 83, 85, 87 };
    /* constant increment per further step */
    static const seq_t delta[6] = { 34, 31, 31, 34, 31, 31 };

    seq_t diff = (seq_t)(k - threshold[r]);
    return base[r] + diff * delta[r];
}
