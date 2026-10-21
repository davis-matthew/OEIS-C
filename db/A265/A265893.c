/*
 * A265893  a(n) = A084558(n) - A230403(n); the length of factorial base
 *          representation of n without its trailing zeros.
 * https://oeis.org/A265893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A084558(n) - A230403(n); Computes the length of the
 *            factorial base representation of n minus the number of its
 *            trailing zero digits
 * Range:     u8 n <= 322, u16 n <= 9895, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                            /* convention: a(0)=0 */
        return (seq_t)0;

    seq_t nn = (seq_t)n;                   /* work in seq_t */
    idx_t  d = 0;                          /* number of factorial digits */
    seq_t  fact = 1;

    /* find largest k with k! <= n  ->  d = k */
    for (idx_t k = 1; ; ++k) {
        fact *= (seq_t)k;                 /* fact = k!   */
        if (fact <= nn)
            d = k;
        else
            break;
    }

    /* find trailing-zero count: largest t with (t+1)! divides n */
    idx_t  t = 0;
    if (d >= 2) {                          /* at least one possible zero */
        fact = 2;                          /* (2)! = 2  */
        for (idx_t k = 2; ; ++k) {       /* k is the minimal factorial index */
            if (n % fact != 0)
                break;                    /* first non‑zero digit */
            ++t;                          /* this place is a trailing zero */
            if (k + 1 == 0)               /* avoid overflow (impossible here) */
                break;
            fact *= (seq_t)(k + 1);        /* compute next factorial (k+1)! */
            if (fact > nn)                /* no larger factorial can divide n   */
                break;
        }
    }

    return (seq_t)(d - t);                  /* desired value */
}
