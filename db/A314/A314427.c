/*
 * A314427  Coordination sequence Gal.5.253.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=8, a(3)=13; * a(n)=a(n-1)+{4,4,7,3,4}[(n-4)
 *            mod 5] for n>=4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)13;

    /* recurrence for n >= 4 */
    static const int pattern[5] = {4, 4, 7, 3, 4};
    const seq_t cycleSum = 22;           /* sum of one pattern cycle */

    idx_t k = n - 4;                     /* number of steps from a(3) */
    seq_t sum = 0;

    /* number of full 5‑term cycles and remaining terms */
    seq_t fullCycles = (k + 1) / 5;
    int remainder   = (int)((k + 1) % 5);

    sum += fullCycles * cycleSum;
    for (int i = 0; i < remainder; ++i) sum += pattern[i];

    return (seq_t)13 + sum;
}
