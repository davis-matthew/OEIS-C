/*
 * A248017  Table T(n,k), n>=1, k>=1, read by antidiagonals: T(n,k) = number
 *          of equivalence classes of ways of placing five 1 X 1 tiles in an n
 *          X k rectangle under all symmetry operations of the rectangle.
 * https://oeis.org/A248017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = T(i,j) read by antidiagonals, where (i,j) are the
 *            antidiagonal coordinates of index n and T(i,j) is given by the
 *            empirical polynomial below
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 574, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert sequence index to rectangle dimensions (i,j) */
    idx_t m = n;
    idx_t s = 2;          /* current sum i + j */
    idx_t cum = 1;        /* cumulative number of terms up to sum s */
    while (cum < m) {
        s++;
        cum += s - 1;     /* add length of this diagonal (s-1) */
    }
    idx_t prev_cum = cum - (s - 1);
    idx_t offset = m - prev_cum - 1;   /* zero‑based offset in this diagonal */
    idx_t i = offset + 1;              /* row index */
    idx_t k = s - offset - 1;          /* column index */

    /* Compute T(i,k) using the polynomial formula, with signed 128‑bit intermediates */
    __int128 N = i;
    __int128 K = k;

    /* powers */
    __int128 N2 = N * N;
    __int128 N3 = N2 * N;
    __int128 N4 = N3 * N;
    __int128 N5 = N4 * N;
    __int128 K2 = K * K;
    __int128 K3 = K2 * K;
    __int128 K4 = K3 * K;
    __int128 K5 = K4 * K;

    /* sign factors for parity terms */
    int signk = (k & 1) ? -1 : 1;
    int signn = (i & 1) ? -1 : 1;
    int signkn = signk * signn;  /* (-1)^(k+n) */

    /* Main polynomial components */
    __int128 term0 =  4 * K5 * N5
                   - 40 * K4 * N4
                   + 140 * K3 * N3
                   + 2 * K5
                   + 20 * K4 * N
                   + 30 * K3 * N2
                   + 30 * K2 * N3
                   + 20 * K * N4
                   + 2 * N5
                   - 40 * K4
                   - 120 * K3 * N
                   - 185 * K2 * N2
                   - 120 * K * N3
                   - 40 * N4
                   + 160 * K3
                   - 20 * K2 * N
                   - 20 * K * N2
                   + 160 * N3
                   - 80 * K2
                   + 36 * K * N
                   - 80 * N2
                   + 48 * K
                   + 48 * N
                   + 45;

    __int128 term1 = (-30 * K2 * N3
                    - 20 * K * N4
                    - 2 * N5
                    - 15 * K2 * N2
                    + 120 * K * N3
                    + 40 * N4
                    + 20 * K * N2
                    - 160 * N3
                    + 60 * K * N
                    + 80 * N2
                    - 48 * N
                    - 45);
    if (signk < 0) term1 = -term1;

    __int128 term2 = (-2 * K5
                    - 20 * K4 * N
                    - 30 * K3 * N2
                    + 40 * K4
                    + 120 * K3 * N
                    - 15 * K2 * N2
                    - 160 * K3
                    + 20 * K2 * N
                    + 80 * K2
                    + 60 * K * N
                    - 48 * K
                    - 45);
    if (signn < 0) term2 = -term2;

    __int128 term3 = (15 * K2 * N2
                    - 60 * K * N
                    + 45);
    if (signkn < 0) term3 = -term3;

    __int128 result = term0 + term1 + term2 + term3;
    result /= 1920;   /* guaranteed to be integer */

    return (seq_t)result;
}
