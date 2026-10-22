/*
 * A309300  (1/8) times the sum of the elements of all subsets of [n] whose
 *          sum is divisible by eight.
 * https://oeis.org/A309300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8) * Σ_{S⊆{1..n}, ΣS≡0 (mod 8)} ΣS. Recurrence (for n
 *            ≥ 14): a(n)=6*a(n-1)−12*a(n-2)+8*a(n-3). Initial terms:
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=1,a(5)=3,a(6)=10,
 *            a(7)=27,a(8)=70,a(9)=178,a(10)=438,a(11)=1056,a(12)=2496,a(13)=5824
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 30, u64 n <= 60, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[14] = {
        0, 0, 0, 0, 1, 3, 10, 27, 70, 178, 438, 1056, 2496, 5824
    };

    if (n < 14)
        return init[(idx_t)n];

    /* use the last three known values as the base of the recurrence */
    seq_t a0 = init[11]; /* a(n-3) when n == 14 */
    seq_t a1 = init[12]; /* a(n-2) when n == 14 */
    seq_t a2 = init[13]; /* a(n-1) when n == 14 */

    for (idx_t i = 14; i <= n; ++i) {
        seq_t next = (seq_t)6 * a2 - (seq_t)12 * a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
