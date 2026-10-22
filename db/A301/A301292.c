/*
 * A301292  Partial sums of A301291.
 * https://oeis.org/A301292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n*n + 9*n + 5 + δ(n)) / 4 where δ(n) = +1 for n ≡ 1 or
 *            2 (mod 4), δ(n) = –1 for n ≡ 0 or 3 (mod 4)
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using the 64‑bit idx_t type (guaranteed to hold n≤10000). */
    idx_t nn = n;               /* local copy of the index */
    idx_t val = 9 * nn * nn + 9 * nn + 5;   /* 9*n^2 + 9*n + 5 */

    /* Adjust by ±1 according to n mod 4. */
    switch (nn & 3u) {
        case 0:  val -= 1;  break;   /* n ≡ 0 (mod 4) → δ = –1 */
        case 3:  val -= 1;  break;   /* n ≡ 3 (mod 4) → δ = –1 */
        case 1:  val += 1;  break;   /* n ≡ 1 (mod 4) → δ = +1 */
        case 2:  val += 1;  break;   /* n ≡ 2 (mod 4) → δ = +1 */
    }

    return (seq_t)(val / 4);   /* exact division by 4 */
}
