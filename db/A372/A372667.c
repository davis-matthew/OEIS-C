/*
 * A372667  Norm i^2+j^2+k^2 of (i,j,k) for 0 <= k <= j <= i.
 * https://oeis.org/A372667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = i^2 + j^2 + k^2 where (i,j,k) is the (n+1)-th triple with
 *            0 <= k <= j <= i, enumerated by increasing i, then j, then k
 * Range:     u8 n <= 272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find i such that C(i-1) <= n < C(i)
       where C(i) = (i+1)(i+2)(i+3)/6. */
    idx_t i = 0;
    seq_t Ci;
    while (1) {
        seq_t ip1 = (seq_t)(i + 1);
        seq_t ip2 = (seq_t)(i + 2);
        seq_t ip3 = (seq_t)(i + 3);
        Ci = ip1 * ip2 * ip3 / 6;
        if ((seq_t)n < Ci)
            break;
        ++i;
    }

    /* Compute cumulative count up to previous i (C(i-1)). */
    seq_t Cprev;
    if (i == 0) {
        Cprev = 0;
    } else {
        seq_t ip0 = (seq_t)i;
        seq_t ip1 = (seq_t)(i + 1);
        seq_t ip2 = (seq_t)(i + 2);
        Cprev = ip0 * ip1 * ip2 / 6;
    }

    /* Offset within the group with this i. */
    idx_t offset = n - (idx_t)Cprev;

    /* Find j such that offset falls within its block. */
    idx_t j = 0;
    while (offset >= (idx_t)(j + 1)) {
        offset -= (idx_t)(j + 1);
        ++j;
    }

    /* Remaining offset gives k. */
    idx_t k = offset;

    /* Compute a(n) = i^2 + j^2 + k^2. */
    seq_t val = (seq_t)i * i + (seq_t)j * j + (seq_t)k * k;
    return val;
}
