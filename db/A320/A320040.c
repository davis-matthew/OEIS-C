/*
 * A320040  Consider the Cantor matrix of rational numbers. This sequence
 *          reads the numerator, then the denominator as one moves through the
 *          matrix along alternate up and down antidiagonals.
 * https://oeis.org/A320040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Cantor matrix enumeration: a(n) gives the nth numerator or denominator
   when traversing the matrix of fractions i/j along anti‑diagonals
   with alternating direction. The formula for a given n is:
   Let s be the smallest integer such that (s‑1)*s ≥ n.
   Let p = n – (s‑2)*(s‑1) – 1          // 0‑based offset in the diagonal
   Let f = p/2, r = p%2                 // f is fraction index, r is position
   Numerator = (s odd) ? 1 + f : (s‑1) – f
   a(n) = (r==0) ? Numerator : s – Numerator.
*/
seq_t a(idx_t n)
{
    /* Guard against an illegal index; behaviour undefined for n < 1. */
    if (n == 0) return 0;  

    /* Find the diagonal s such that (s‑1)*s ≥ n. */
    idx_t low = 1, high = 2;
    while ((unsigned __int128)(high - 1) * high < n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)(mid - 1) * mid >= n) {
            high = mid;
        } else {
            low = mid;
        }
    }
    idx_t s = high;  /* diagonal sum */

    /* Previous number of terms before this diagonal. */
    unsigned __int128 prev = (unsigned __int128)(s - 2) * (s - 1);
    idx_t offset = (idx_t)(n - (idx_t)prev - 1); /* 0‑based within diagonal */

    idx_t f = offset >> 1;      /* fraction index within diagonal */
    idx_t r = offset & 1;       /* 0 – numerator, 1 – denominator */

    seq_t i;
    if (s & 1) {                /* odd diagonal – increasing i */
        i = (seq_t)(1 + f);
    } else {                    /* even diagonal – decreasing i */
        i = (seq_t)((s - 1) - f);
    }

    return (r == 0) ? i : (seq_t)(s - i);
}
