/*
 * A268539  Numbers k such that 48*k + 25 is a perfect square.
 * https://oeis.org/A268539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268539: Numbers k such that 48*k+25 is a perfect square.
   Formula used: let L = {5,11,13,19,29,35,37,43}.
   For n ≥ 1:
           j = (n-1)/8                 // number of full 8‑term blocks
           i = (n-1)%8                 // position inside a block
           m = L[i] + 48*j
           a(n) = (m*m - 25)/48
   This directly computes a(n) in O(1) time without recursion or memoisation. */

seq_t a(idx_t n)
{
    /* handle non‑positive index gracefully (though not required) */
    if (n == 0) {
        return 0;
    }

    const seq_t L[8] = {5, 11, 13, 19, 29, 35, 37, 43};

    idx_t j = (n - 1) / 8;          /* block number */
    idx_t i = (n - 1) % 8;          /* index within block */

    seq_t m = (seq_t)L[i] + (seq_t)48 * (seq_t)j;  /* compute m */
    seq_t m2 = m * m;                               /* m squared */

    return (m2 - (seq_t)25) / (seq_t)48;
}
