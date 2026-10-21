/*
 * A265330  Zero-based row index to A265345; 2-adic valuation of bijective
 *          base-3 reversal of n: a(n) = A007814(A263273(n)).
 * https://oeis.org/A265330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2-adic valuation of the bijective base‑3 reversal of n *
 *            i.e. a(n) = A007814(A263273(n)) * Equivalent implementation:
 *            a(n) = v₂(reverse_base3_digits(n)) * * reverse_base3_digits(n):
 *            iterate over the base‑3 digits of n from * least significant to
 *            most significant, building a number whose di...
 * Range:     u8 n <= 6561, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is a 1‑based index; if called with n==0, return 0. */
    if (n == 0)
        return 0;

    idx_t t = n;              /* Work copy of the index.           */
    seq_t rev = 0;            /* Reversed base‑3 number.           */

    /* Build the reverse of n's base‑3 representation. */
    while (t > 0) {
        /* Current (least significant) base‑3 digit. */
        unsigned int digit = (unsigned int)(t % 3);
        rev = rev * 3 + digit;  /* Append digit to the reversed number. */
        t /= 3;
    }

    /* Compute the 2‑adic valuation of the reversed number. */
    seq_t cnt = 0;
    while ((rev % 2) == 0) {   /* While rev is even: divide by 2. */
        rev = rev / 2;
        cnt += 1;
    }

    return cnt;
}
