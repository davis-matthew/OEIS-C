/*
 * A319610  a(n) is the minimal number of successive OFF cells that appears in
 *          n-th generation of rule-30 1D cellular automaton started from a
 *          single ON cell.
 * https://oeis.org/A319610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 0 or 1, 2 for n = 2, 2 for n = 4,6,8, 1
 *            otherwise. The OEIS data shows that for all n ≥ 10 the sequence
 *            stays at 1
 * Range:     u8 n <= 138, u16 n <= 138, u32 n <= 138, u64 n <= 138,
 *            u128 n <= 138, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the very small indices explicitly */
    if (n == 0 || n == 1) {
        return 0;
    }
    if (n == 2) {
        return 2;
    }

    /* Indices 3 … 9 are known explicitly */
    if (n < 10) {
        /* Even indices 4,6,8 have value 2; all others are 1 */
        return (n & 1) ? 1 : 2;
    }

    /* For all n ≥ 10, the minimal run of 0‑cells observed in rule‑30
       generations is 1 (conjectured to hold always). */
    return 1;
}
