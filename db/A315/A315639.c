/*
 * A315639  Coordination sequence Gal.4.74.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n ≥ 1: a(n) = a(n-1) + d(n), where d(1)=5; for n ≥
 *            2, d(n)=4 if n ≡ 3 or 4 (mod 6), otherwise d(n)=6. (Thus the
 *            increments are: 5,6,4,4,6,6,6,6,… with the 4's recurring every
 *            pair in positions 3–4 of each 6‑term block.)
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t first_cycle[6] = {5, 6, 4, 4, 6, 6};
    const seq_t second_cycle[6] = {6, 6, 4, 4, 6, 6};
    seq_t result = 1;                 /* a(0) */

    if (n == 0) return result;        /* a(0) */

    if (n <= 6) {
        for (idx_t i = 0; i < n; ++i)
            result += first_cycle[i];
        return result;
    }

    /* Add the first 6 increments: 5+6+4+4+6+6 = 31 */
    result += 31;

    idx_t rem = n - 6;                /* remaining indices after the first block */
    idx_t cycles = rem / 6;
    idx_t tail  = rem % 6;

    /* Each full 6‑cycle after the first contributes 32 (6+6+4+4+6+6) */
    result += (seq_t)cycles * 32;

    /* Add the remaining tail increments */
    for (idx_t i = 0; i < tail; ++i)
        result += second_cycle[i];

    return result;
}
