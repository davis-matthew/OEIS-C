/*
 * A278612  Sum of terms in level n of TRIP - Stern sequence associated with
 *          permutation triple (e, e, e).
 * https://oeis.org/A278612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 4*a(n-3); Initial values: a(0)=3,
 *            a(1)=8, a(2)=22
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)22;

    /* Use three rolling variables to avoid any dynamic memory allocation.
       These are sufficient because the recurrence is of order three. */
    seq_t a0 = (seq_t)3;   /* a(k-2) when k = 3 */
    seq_t a1 = (seq_t)8;   /* a(k-1) when k = 3 */
    seq_t a2 = (seq_t)22;  /* a(k)   when k = 3 */

    for (idx_t k = 3; k <= n; ++k) {
        seq_t new_val = 4 * a2 - 5 * a1 + 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = new_val;
    }
    return a2; /* a(n) after the loop ends */
}
