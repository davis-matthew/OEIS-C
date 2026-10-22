/*
 * A337416  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defined by a complete bipartite graph K(5,n)
 *          (with n at least 3) missing two edges, where the removed edges are
 *          incident to the same point in the 5 point part.
 * https://oeis.org/A337416
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 225*31^(n-2) - 421*15^(n-2) + 250*7^(n-2) - 58*3^(n-2) +
 *            4
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent 5‑term linear recurrence for n>7: */
/*  a(n) = 57*a(n-1) - 1002*a(n-2) + 6562*a(n-3) - 15381*a(n-4) + 9765*a(n-5) */
/* Initial values: a(3)=2240, a(4)=133232, a(5)=5366288, a(6)=187074656, a(7)=6126049760 */

seq_t a(idx_t n)
{
    /* Off‑by‑two offset: ignore n < 3 */
    if (n < 3) return 0;

    /* Direct return for the first five known terms */
    if (n == 3) return (seq_t)2240;
    if (n == 4) return (seq_t)133232;
    if (n == 5) return (seq_t)5366288;
    if (n == 6) return (seq_t)187074656;
    if (n == 7) return (seq_t)6126049760;

    /* Otherwise use the recurrence.  We keep the last five terms */
    seq_t a1 = (seq_t)2240;        /* a(n-5)  */
    seq_t a2 = (seq_t)133232;      /* a(n-4)  */
    seq_t a3 = (seq_t)5366288;     /* a(n-3)  */
    seq_t a4 = (seq_t)187074656;   /* a(n-2)  */
    seq_t a5 = (seq_t)6126049760;  /* a(n-1)  */
    seq_t next = 0;

    for (idx_t i = 8; i <= n; ++i) {
        /* Compute 57*a5 + 6562*a3 + 9765*a1  - 1002*a4 - 15381*a2 */
        seq_t pos = (seq_t)57 * a5 + (seq_t)6562 * a3 + (seq_t)9765 * a1;
        seq_t neg = (seq_t)1002 * a4 + (seq_t)15381 * a2;
        next = pos - neg;   /* a(n) is guaranteed positive */

        /* Shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
