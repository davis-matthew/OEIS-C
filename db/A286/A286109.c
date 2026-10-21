/*
 * A286109  Square array read by antidiagonals: A(n,k) = T(n XOR k, 2*(n AND
 *          k)), where T(n,k) is sequence A001477 considered as a
 *          two-dimensional table, AND is bitwise-and (A004198) and XOR is
 *          bitwise-xor (A003987).
 * https://oeis.org/A286109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = CantorPairing( (i ^ j), 2 * (i & j) ), where (i , j) are
 *            the antidiagonal coordinates of n
 * Range:     u8 n <= 252, u16 n <= 9902, u32 n <= 9902, u64 n <= 9902,
 *            u128 n <= 9902, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t find_diagonal(idx_t n)
{
    /* Find a high bound by doubling until the triangular number exceeds n. */
    idx_t low = 0;
    idx_t high = 1;
    while (((unsigned __int128)high * (high + 1) / 2) <= n)
        high <<= 1;

    /* Binary search for the largest d with d*(d+1)/2 <= n. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* 1. Diagonal index and offset within that diagonal. */
    idx_t d = find_diagonal(n);
    idx_t tri = (idx_t)(((unsigned __int128)d * (d + 1)) / 2);
    idx_t offset = n - tri;

    /* 2. Antidiagonal coordinates. */
    idx_t i = offset;
    idx_t j = d - offset;

    /* 3. Arguments for the Cantor pairing function. */
    seq_t a_val = (seq_t)(i ^ j);           /* first argument */
    seq_t b_val = (seq_t)((i & j) << 1);    /* second argument */

    /* 4. Cantor pairing: ((a+b)*(a+b+1))/2 + a. */
    seq_t sum = a_val + b_val;
    seq_t prod = sum * (sum + (seq_t)1) / (seq_t)2;
    seq_t result = prod + a_val;

    return result;
}
