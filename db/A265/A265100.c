/*
 * A265100  a(n) = 9*A005836(n) + 5, n >= 1.
 * https://oeis.org/A265100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A265100: a(n) = 9 * A005836(n) + 5
 * A005836(n) is the (n‑1)-th non‑negative integer that contains
 * only 0's and 1's in its base‑3 representation.  Equivalently,
 * take the binary representation of n‑1 and interpret its bits as
 * digits in base 3.  For example, n = 5 → n‑1 = 4 = 100₂
 * → 100₃ = 9.  Then a(5) = 9·9 + 5 = 86.  */
seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1; undefined for n=0 */
        return 0;

    idx_t x = n - 1;   /* binary number to be interpreted as base‑3 digits */
    seq_t res  = 0;    /* result of interpreting x in base 3 */
    seq_t pow3 = 1;    /* current power of 3, 3^i */

    while (x) {
        if (x & 1U)             /* bit i of x is 1 → add 3^i */
            res += pow3;
        pow3 *= 3;               /* next power of 3 */
        x >>= 1;                 /* move to next bit */
    }

    /* a(n) = 9 * res + 5 */
    return res * (seq_t)9 + (seq_t)5;
}
