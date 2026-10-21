/*
 * A280046  Image of A053838 under the morphism 0 -> 0000, 1 -> 0100, 2 ->
 *          0110.
 * https://oeis.org/A280046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 except * * if floor(n/4) has a base‑3 digit sum
 *            congruent to 1 (mod 3) and n%4==1, then a(n)=1, * * if
 *            floor(n/4) has a base‑3 digit sum congruent to 2 (mod 3) and
 *            n%4∈{1,2}, then a(n)=1, * * otherwise a(n)=0. This implements
 *            the image of A053838 under the morphism * * 0→0000, 1→0100,
 *            2→...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of sequence A280046 */
seq_t a(idx_t n)
{
    /* Determine which block (symbol of A053838) covers the desired position. */
    idx_t block = n / 4;            /* floor(n/4)   */
    idx_t offset = n % 4;           /* n modulo 4   */

    /* Compute the digit sum modulo 3 of the block index in base 3. */
    idx_t t = block;
    idx_t sum = 0;                  /* will stay in {0,1,2} */
    while (t) {
        sum += t % 3;               /* add the least significant base‑3 digit */
        if (sum >= 3) sum -= 3;     /* keep it modulo 3 */
        t /= 3;
    }

    /* Map the symbol (sum mod 3) to the appropriate block of 4 bits. */
    if (sum == 0) {
        /* 0 → 0000 */
        return 0;
    } else if (sum == 1) {
        /* 1 → 0100 : only the 2nd position (offset==1) is 1 */
        return (seq_t)(offset == 1);
    } else { /* sum == 2 */
        /* 2 → 0110 : the 2nd and 3rd positions (offset==1 or offset==2) are 1 */
        return (seq_t)((offset == 1) || (offset == 2));
    }
}
