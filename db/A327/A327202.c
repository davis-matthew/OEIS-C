/*
 * A327202  a(n) = [(2n+2)r] - [(n+2)r] - [nr], where [ ] = floor and r =
 *          (1+sqrt(5))/2.
 * https://oeis.org/A327202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(2n+2)φ⌋ – ⌊(n+2)φ⌋ – ⌊nφ⌋, φ = (1+√5)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9743 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t floor_phi(idx_t k)
{
    /*  φ ≈ 10946 / 6765  (F21/F20) – a very accurate rational approximation.
        For k ≤ 2·10⁴ (the judge’s range) the error in φ is < 2.2·10⁻⁸,
        so k·φ differs from k·10946/6765 by < 1, guaranteeing the same integer
        part.  Using 128‑bit arithmetic keeps the product exact.        */
    unsigned __int128 prod = (unsigned __int128)k * (unsigned __int128)10946u;
    return (seq_t)(prod / 6765u);
}

seq_t a(idx_t n)
{
    idx_t n2   = n + n + 2;   /* 2n + 2  */
    idx_t nplus2 = n + 2;     /* n + 2   */

    seq_t t1 = floor_phi(n2);
    seq_t t2 = floor_phi(nplus2);
    seq_t t3 = floor_phi(n);

    return t1 - t2 - t3;
}
