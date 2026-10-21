/*
 * A296349  Position where the row corresponding to the binary expansion of n
 *          starts in the (flattened) binary Champernowne triangle A030190.
 * https://oeis.org/A296349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{k=0}^{n-1} (k==0 ? 1 : floor(log2(k))+1)
 * Range:     u8 n <= 52, u16 n <= 5671, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Sum of bit lengths of numbers 0 … n-1. */
    idx_t nn = n - 1;
    seq_t total = 1;          /* the length of the number 0 */

    unsigned int d = 1;
    while (1) {
        idx_t start = (idx_t)1 << (d - 1);   /* first number with d bits */
        if (start > nn) break;               /* no more numbers to add */

        idx_t end;
        if (d == 64) {
            end = nn;                       /* 2^64-1 is the maximum idx_t value */
        } else {
            idx_t limit = ((idx_t)1 << d) - 1;
            end = (nn < limit) ? nn : limit;/* last number with d bits, limited by n-1 */
        }

        idx_t cnt = end - start + 1;        /* how many numbers have d bits */
        total += (seq_t)d * (seq_t)cnt;     /* add their bit lengths */

        if (d == 64) break;                 /* avoid shifting by 64 bits */
        ++d;
    }

    return total;
}
