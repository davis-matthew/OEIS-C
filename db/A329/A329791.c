/*
 * A329791  a(n) = floor(sqrt(2)*n) + floor(sqrt(3)*n).
 * https://oeis.org/A329791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2)*n) + floor(sqrt(3)*n)
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 853 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sqrt2_floor(idx_t n)
{
    idx_t lo = 0, hi = n * 2 + 1;                 /* hi sufficiently large */
    unsigned __int128 rhs = (unsigned __int128)2 * n * n; /* rhs = 2·n² */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;          /* upper mid */
        unsigned __int128 lhs = (unsigned __int128)mid * mid;
        if (lhs <= rhs)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;
}

static seq_t sqrt3_floor(idx_t n)
{
    idx_t lo = 0, hi = n * 2 + 1;                 /* hi sufficiently large */
    unsigned __int128 rhs = (unsigned __int128)3 * n * n; /* rhs = 3·n² */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;          /* upper mid */
        unsigned __int128 lhs = (unsigned __int128)mid * mid;
        if (lhs <= rhs)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (seq_t)lo;
}

seq_t a(idx_t n)
{
    return sqrt2_floor(n) + sqrt3_floor(n);
}
