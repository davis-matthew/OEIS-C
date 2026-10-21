/*
 * A290260  a(n) = number of isolated 0's in the binary representation of n.
 * https://oeis.org/A290260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8786, u16 n <= 8786, u32 n <= 8786, u64 n <= 8786,
 *            u128 n <= 8786, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of isolated 0's in the binary representation of n.
 *
 * Recurrence:
 *   a(1) = 0
 *   If n is odd      -> a(n) = a(n>>1)
 *   If n is even:
 *     let k = n>>1
 *     if k is odd            -> a(n) = a(k) + 1
 *     else if k%4 == 2 (k≡2) -> a(n) = a(k) - 1
 *     else (k%4 == 0)       -> a(n) = a(k)
 *
 * This implements the OEIS formulas in a closed recursion suitable for
 * any n≥1, giving the number of isolated 0‑bits.
 */

seq_t a(idx_t n)
{
    if (n == 1ull)
        return 0;

    if (n & 1ull)                    /* n is odd */
        return a(n >> 1);

    /* n is even */
    idx_t k   = n >> 1;              /* k = n / 2 */
    seq_t val = a(k);                /* recursion on the halved value */

    if (k & 1ull)                    /* k odd  -> a(2k) = a(k)+1 */
        return val + 1;

    if ((k & 3ull) == 2ull)          /* k ≡ 2 (mod 4) -> a(2k)=a(k)-1 */
        return val - 1;

    /* k ≡ 0 (mod 4) -> a(2k)=a(k) */
    return val;
}
