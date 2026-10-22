/*
 * A309590  Inverse of A309589: 0-based positions in the number subsets {0,
 *          ..., 10^k - 1} written in base 10 and sorted lexicographically,
 *          for k = 1, 2, ...
 * https://oeis.org/A309590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   a(n) returns the lexicographic rank of the number i in the * set
 *            {0,1,…,10^k-1} where i is the n‑th term (in 0‑based order) of
 *            the * concatenated rows k = 1,2,… of all numbers sorted
 *            lexicographically. * The rank is the number of strings from
 *            0..10^k-1 that are lexicographically * smaller than th...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ---------- find k and the offset i within the row ---------- */
    idx_t remaining = n;
    idx_t k = 1;
    idx_t pow10k = 10;
    while (remaining >= pow10k) {
        remaining -= pow10k;
        ++k;
        pow10k *= 10;          /* pow10k = 10^k after the loop */
    }
    idx_t i = remaining;       /* i ∈ [0,10^k‑1] */

    /* ---------- decimal digits of i (most significant first) --------- */
    int digits[32];
    int len = 0;
    if (i == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = i;
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to most‑significant first */
        for (int a = 0, b = len - 1; a < b; ++a, --b) {
            int t = digits[a];
            digits[a] = digits[b];
            digits[b] = t;
        }
    }

    /* ---------- pre‑compute powers of 10 up to k (seq_t) ----------- */
    seq_t pow10_arr[64];
    pow10_arr[0] = (seq_t)1;
    int kk = (int)k;                 /* k fits in an int for our use cases */
    for (int e = 1; e <= kk; ++e)
        pow10_arr[e] = pow10_arr[e - 1] * (seq_t)10;

    /* ---------- compute lexicographic rank (0‑based) --------------- */
    seq_t rank = 0;
    for (int j = 0; j < len; ++j) {
        int dcur = digits[j];
        for (int d = 0; d < dcur; ++d) {
            if (j == 0 && d == 0) {
                /* the number 0 alone */
                rank += (seq_t)1;
            } else {
                /* numbers starting with the current prefix + d,
                   followed by any suffix up to length k-(j+1) */
                seq_t cnt = (pow10_arr[kk - j] - (seq_t)1) / (seq_t)9;
                rank += cnt;
            }
        }
    }
    /* add numbers that are strict prefixes of i (valid numbers)      */
    if (len > 1)
        rank += (seq_t)(len - 1);

    return rank;
}
