/*
 * A374584  Numbers k such that 7*k + 2 is a square.
 * https://oeis.org/A374584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((7*((n-1)/2)+((n%2)?3:4))^2 - 2)/7
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* Offset is 1; guard against n==0 */
    idx_t half = (n - 1) / 2;          /* block number (0‑based)   */
    seq_t base = 7 * (seq_t)half;      /* 7 * block number         */
    seq_t s = base + ((n & 1) ? 3 : 4);/* s(n): 3 or 4 mod 7       */
    seq_t s2 = s * s;                  /* s^2                       */
    seq_t k = (s2 - 2) / 7;            /* (s^2-2)/7                 */
    return k;
}
