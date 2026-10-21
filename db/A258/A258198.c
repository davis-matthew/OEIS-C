/*
 * A258198  a(n) = largest k for which A001563(k) = k*k! <= n.
 * https://oeis.org/A258198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 766, u16 n <= 4320, u32 n <= 4320, u64 n <= 4320,
 *            u128 n <= 4320, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A258198: a(n) = largest k for which k*k! <= n. */

seq_t a(idx_t n)
{
    /* Convert the index to sequence type */
    seq_t lim = (seq_t)n;
    unsigned __int128 limit = (unsigned __int128)lim;

    unsigned __int128 fact = 1;        /* k!  */
    for (idx_t k = 1;; ++k) {
        fact *= (unsigned __int128)k;          /* fact = k! */
        unsigned __int128 prod = fact * (unsigned __int128)k; /* k*k! */
        if (prod > limit)
            return (seq_t)(k - 1);
    }
}
