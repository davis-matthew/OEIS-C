/*
 * A256676  Digital roots of centered 11-gonal numbers (A069125).
 * https://oeis.org/A256676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root( A069125(n) ) = (n*(n-1)+1) mod 9, with 0
 *            mapped to 9 (although it never occurs for n≥1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 9 to avoid large intermediate products. */
    idx_t r  = n % 9;                 /* n mod 9 */
    idx_t r2 = (n + 8) % 9;           /* (n-1) mod 9, avoid underflow */
    idx_t prod = (r * r2) % 9;        /* (n*(n-1)) mod 9 */
    idx_t val  = (prod + 1) % 9;      /* (n*(n-1)+1) mod 9 */
    if (val == 0)
        val = 9;                     /* map 0 → 9 for digital‑root convention,
                                         though this case never occurs for n≥1 */
    return (seq_t)val;
}
