/*
 * A272868  Triangle read by rows, T(n,k) = 2^k*GegenbauerC(k,-n,-1/4), for
 *          n>=0 and 0<=k<=n.
 * https://oeis.org/A272868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:  a(n) = T(m,k) with
 *   T(m,k) = 2^k * GegenbauerC(k,-m,-1/4).
 * For a given index n the coordinates are found from
 *   n = m*(m+1)/2 + k , 0 <= k <= m .
 * T(0,0)=1, T(m,0)=1, T(m,1)=m.
 * For k>=2 the recurrence
 *   T(m,k) = ((m-(k-1))*T(m,k-1) + 4*((2*m+1)-(k-1))*T(m,k-2)) / k
 * is used.  All operations are on integers of type seq_t. */

seq_t a(idx_t n)
{
    /* map n to (m,k) */
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    idx_t lo = high >> 1, hi = high;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) lo = mid;
        else hi = mid - 1;
    }
    idx_t m = lo;
    idx_t k = n - (unsigned __int128)m * (m + 1) / 2;

    if (k == 0) return (seq_t)1;
    if (k == 1) return (seq_t)m;

    seq_t v_prev = (seq_t)1;   /* T(m,0) */
    seq_t v_curr = (seq_t)m;   /* T(m,1) */
    for (idx_t j = 1; j < k; ++j) {
        /* j corresponds to current index k=j */
        seq_t term1 = ((seq_t)m - (seq_t)j) * v_curr;
        seq_t diff_n = ((seq_t)2 * (seq_t)m + (seq_t)1) - (seq_t)j;
        seq_t term2 = diff_n * v_prev * (seq_t)4;
        seq_t numerator = term1 + term2;
        seq_t v_next = numerator / ((seq_t)(j + 1));
        v_prev = v_curr;
        v_curr = v_next;
    }
    return v_curr;
}
