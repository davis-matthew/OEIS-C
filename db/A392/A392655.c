/*
 * A392655  Binary-encoded contents of pin 2, after n moves, when moving
 *          infinitely many disks in the Tower of Hanoi starting from pin 1,
 *          via the optimal algorithm.
 * https://oeis.org/A392655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{i≥0} 2^i * [ ((⌊(n + 2^i)/2^(i+1)⌋) mod 3) ==
 *            (i%2==0 ? 1 : 2) ] * i.e. the i-th bit of a(n) is 1 iff disk i
 *            is on pin 2 after n moves
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 0; (n >> i) != 0; ++i) {
        /* number of moves disk i has performed up to step n */
        idx_t q = n >> (i + 1);               /* floor(n / 2^(i+1)) */
        idx_t q_mod3 = q % 3;

        /* compute remainder r = n mod 2^(i+1) without shifting by 64 */
        idx_t mask;
        if (i + 1 == 64u) {
            mask = ~(idx_t)0;
        } else {
            mask = ((idx_t)1 << (i + 1)) - 1;
        }
        idx_t r = n & mask;

        /* does the remainder exceed 2^i ? */
        idx_t wrap = (r >= ((idx_t)1 << i)) ? 1u : 0u;

        idx_t k_mod3 = (q_mod3 + wrap) % 3;

        /* include disk i on pin 2 ?
         * even i -> need k_mod3 == 1
         * odd  i -> need k_mod3 == 2
         */
        int include = ((i & 1u) == 0u && k_mod3 == 1u) ||
                      ((i & 1u) != 0u && k_mod3 == 2u);

        if (include) {
            result |= ((seq_t)1 << i);
        }
    }
    return result;
}
