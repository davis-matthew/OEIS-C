/*
 * A310418  Coordination sequence Gal.3.43.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For Gal.3.43.1 coordination sequence * * a(0) = 1 * a(1) = 4 *
 *            For even n = 2k (k ≥ 1): a(2k) = 10·k + 2·⌊(k‑1)/3⌋ * For odd n
 *            = 2k+1 (k ≥ 1): a(2k+1) = 14 + 28·⌊(k‑1)/3⌋ + * { 0 if (k‑1) mod
 *            3 = 0 * 12 if (k‑1) mod 3 = 1 * 20 if (k‑1) mod 3 = 2 }
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Short cases that avoid underflow in the formula */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    if ((n & 1ull) == 0)        /* even n */
    {
        seq_t k = (seq_t)(n / 2);          /* k ≥ 1 */
        seq_t part = (k - 1) / 3;           /* floor((k-1)/3) */
        return (seq_t)10 * k + (seq_t)2 * part;
    }
    else                        /* odd n */
    {
        seq_t k = (seq_t)((n - 1) / 2);       /* k ≥ 1 */
        seq_t cycles = (k - 1) / 3;           /* floor((k-1)/3) */
        seq_t r = (k - 1) % 3;
        seq_t remainder = (seq_t)((r == 1) ? 12 :
                                 (r == 2) ? 20 : 0);
        return (seq_t)14 + (seq_t)28 * cycles + remainder;
    }
}
