/*
 * A340260  T(n, k) = Sum_{j=1..k} [n mod j <> 1], where [] is the Iverson
 *          bracket. Table read by rows.
 * https://oeis.org/A340260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row‑major entry of the triangle * T(n,k) = Sum_{j=1..k} [
 *            n mod j != 1 ] * The sequence is obtained by flattening the
 *            triangle row by row
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t N) {
    /* Find the row r such that r*(r+1)/2 >= N */
    idx_t low = 0, high = 1;
    unsigned __int128 sum;
    while ((sum = (unsigned __int128)high * (high + 1) / 2) < N)
        high <<= 1;
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= N)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;                      /* row index (>=1) */

    /* Compute column k = N - r*(r-1)/2 */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = N - (idx_t)prev;            /* 1 <= k <= r */

    /* Count how many j in [1,k] satisfy r mod j == 1 */
    idx_t bad = 0;
    for (idx_t j = 1; j <= k; ++j)
        if (r % j == 1)
            ++bad;

    /* Result is k - bad */
    return (seq_t)(k - bad);
}
