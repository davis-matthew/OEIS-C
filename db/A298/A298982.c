/*
 * A298982  a(n) is the least k for which the most significant decimal digits
 *          of k/n (disregarding any leading zeros) are n, or 0 if no such k
 *          exists.
 * https://oeis.org/A298982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the least k < n such that floor(10^m*k/n) = n for some m
 *            (m must be at least the number of decimal digits of n). If no
 *            such k exists, a(n) = 0
 * Range:     u8 n <= 14, u16 n <= 3025, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0, an empty search yields 0 */

    seq_t best_k = 0;

    /* number of decimal digits of n */
    idx_t tmp = n;
    int d = 0;
    do {
        ++d;
        tmp /= 10;
    } while (tmp != 0);

    /* 10^d */
    seq_t pow10_m = 1;
    for (int i = 0; i < d; ++i)
        pow10_m *= 10;

    seq_t nseq     = (seq_t)n;
    seq_t upper_lim = nseq * (nseq + 1) - 1;

    for (int m = d; ; ++m) {
        if (pow10_m > upper_lim)
            break;                /* 10^m > n(n+1)-1 -> no more candidates */

        /* lower = ceil(n^2 / 10^m) */
        seq_t lower = (nseq * nseq + pow10_m - 1) / pow10_m;

        if (lower < nseq) {        /* must satisfy k < n */
            seq_t upper = (nseq * (nseq + 1) - 1) / pow10_m;   /* floor((n(n+1)-1)/10^m) */

            if (lower <= upper) {
                if (best_k == 0 || lower < best_k)
                    best_k = lower;
                if (best_k == 1)      /* cannot get smaller than 1 */
                    break;
            }
        }

        pow10_m *= 10;             /* next power of 10 for next m */
    }

    return best_k;
}
