/*
 * A385109  If n is 5 (mod 8) then apply n = (n-1)/4 until the result is not
 *          equivalent 5 (mod 8); otherwise a(n) = n.
 * https://oeis.org/A385109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n, unless n ≡ 5 (mod 8); then repeatedly replace n with
 *            (n‑1)/4 until the result is not ≡ 5 (mod 8). The returned value
 *            is the final n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Apply the rule: while n ≡ 5 mod 8, reduce n → (n‑1)/4 . */
    while ((n & 7ULL) == 5ULL) {
        n = (n - 1) >> 2;      /* (n‑1)/4 is equivalent to a right shift by 2 */
    }
    return (seq_t)n;            /* cast from idx_t to seq_t */
}
