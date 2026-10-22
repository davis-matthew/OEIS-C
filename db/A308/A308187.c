/*
 * A308187  Fixed point (beginning with a) of the morphism a -> aab, b -> b,
 *          over the alphabet {a,b} = {0,1}.
 * https://oeis.org/A308187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1254, u16 n <= 9187, u32 n <= 9187, u64 n <= 9187,
 *            u128 n <= 9187, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point morphism a → aab, b → b over alphabet {a,b} = {0,1}.
   The sequence a(n) is 0 when the nth letter of the infinite word
   is 'a' and 1 when it is 'b'. Works for any integer type seq_t. */

seq_t a(idx_t n)
{
    /* handle n = 0 (outside the offset) */
    if (n == 0)
        return 0;

    /* Compute the length of the expansion of 'a' that covers position n.
       For depth d, len = 2^d – 1.  We grow until len ≥ n. */
    seq_t len = 1;                          /* len of a^0 = 1 */
    while (len < (seq_t)n)
        len = len * 2 + 1;                  /* len = 2*len + 1 */

    /* Walk down the substitution tree from this root to the required leaf.
       pos tracks the position within the current subtree. */
    seq_t pos = n;
    while (len > 1) {
        seq_t sub = (len - 1) / 2;          /* length of a^(d‑1) */

        if (pos <= sub) {                  /* first child 'a' */
            len = sub;
        } else if (pos <= sub * 2) {       /* second child 'a' */
            pos -= sub;                    /* adjust position */
            len = sub;
        } else {                           /* trailing 'b' */
            return (seq_t)1;               /* character is 'b' */
        }
    }

    /* reached a leaf 'a' */
    return (seq_t)0;
}
