/*
 * A342138  Array T(n,k) = (n+k)*(3*n+3*k-5)/2 + (3*k+1), read by ascending
 *          antidiagonals.
 * https://oeis.org/A342138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (n+k)*(3*n+3*k-5)/2 + (3*k+1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342138 is the array T(n,k) read by ascending antidiagonals
   (k = 0,1,2,… for each diagonal).  The nth term is obtained by
   locating the diagonal s with (s+1)(s+2)/2 > n, then k = n - s(s+1)/2
   and n = s - k. */

static inline idx_t
isqrt_u128(__uint128_t v)
{
    idx_t lo = 0, hi = (idx_t)-1;          /* hi < 2^64, which is the
                                               maximum possible square root
                                               of any 128‑bit integer. */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v) return mid;
        if (sq < v)   lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;   /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* Find diagonal s such that s(s+1)/2 <= n < (s+1)(s+2)/2 */
    __uint128_t tmp = (__uint128_t)n * 8 + 1;        /* 8*n + 1  */
    idx_t m = isqrt_u128(tmp);
    idx_t s = (m - 1) >> 1;                          /* floor((sqrt(8n+1)-1)/2) */

    idx_t cum_before = s * (s + 1) / 2;              /* terms before diagonal s */
    idx_t k = n - cum_before;                        /* column index (k) */
    /* row index n_i = s - k, not needed explicitly */

    /* Handle special case where the intermediate product becomes negative
       (s == 1).  For all other s the arithmetic stays non‑negative. */
    if (s == 1) {
        return (k == 0) ? (seq_t)0 : (seq_t)3;
    }

    seq_t ss   = (seq_t)s;
    seq_t kseq = (seq_t)k;

    /* T(n,k) = (s*(3s-5))/2 + 3k + 1   where s = n + k */
    seq_t three_ss = ss * 3;
    seq_t num      = ss * (three_ss - 5);   /* integer division is exact
                                               for s >= 2 (numerator even) */
    seq_t part     = num / 2;
    seq_t result   = part + (kseq * 3) + 1;

    return result;
}
