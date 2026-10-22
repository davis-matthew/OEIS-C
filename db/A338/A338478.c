/*
 * A338478  Let b be an odd function such that b(0) = 0, b(1) = 1, and for any
 *          n > 1 such that 3^x < 2*n < 3^(x+1) for some x > 0, b(n) =
 *          b(3^x-n) - 3^x; a(n) = abs(b(n)) for any n >= 0.
 * https://oeis.org/A338478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |b(n)|, where * b(0) = 0, b(1) = 1, * for n > 1 with 3^x
 *            < 2*n < 3^(x+1), * b(n) = b(3^x – n) – 3^x. * The oddness b(−n)
 *            = –b(n) is used internally
 * Range:     u8 n <= 121, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t npos = n;          /* current positive argument for b() */
    int    sign = 1;         /* accumulated sign from oddness */
    signed __int128 offset = 0;  /* running offset b(n) = sign * b(npos) + offset */

    while (npos > (seq_t)1) {
        /* compute largest power of 3 smaller than 2*npos */
        signed __int128 two = ((signed __int128)npos) << 1;
        signed __int128 pow = 1;
        while (pow * 3 < two) pow *= 3;    /* pow = 3^x */

        signed __int128 m = pow - (signed __int128)npos; /* m = 3^x – npos */

        if (m >= 0) {
            /* m >= 0 : b(npos) = b(m) – pow */
            npos = (seq_t)m;
            offset -= (signed __int128)sign * pow;      /* subtract sign*pow */
        } else {
            /* m < 0 : use oddness b(−m) = –b(m) */
            npos = (seq_t)(-m);           /* make it positive */
            offset -= (signed __int128)sign * pow;      /* subtract sign*pow before sign change */
            sign = -sign;                 /* flip sign due to oddness */
        }
    }

    /* base cases: b(0) = 0, b(1) = 1 */
    seq_t base = (npos == (seq_t)0) ? (seq_t)0 : (seq_t)1;
    signed __int128 result = (signed __int128)sign * (signed __int128)base + offset;
    if (result < 0) result = -result;   /* absolute value */

    return (seq_t)result;
}
