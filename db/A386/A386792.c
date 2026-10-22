/*
 * A386792  Antihydra, a BB(6) Turing machine (values of a).
 * https://oeis.org/A386792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 49, u64 n <= 104, u128 n <= 213,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386792 – Antihydra
   a(0) = 8
   a(n) = ((a(n‑1) / 2) * 3) + (a(n‑1) % 2)   for n ≥ 1
   (follows f(a,b) = (3a/2,b+2) if a even, ((3a–1)/2,b–1) if a odd)
*/
seq_t a(idx_t n)
{
    seq_t val = 8;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t half = val / 2;
        seq_t parity = val % 2;
        val = half * 3 + parity;
    }
    return val;
}
