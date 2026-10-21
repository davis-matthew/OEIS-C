/*
 * A253586  The sum of the i-th ternary digits of n, k, and A(n,k) equals 0
 *          (mod 3) for each i>=0 (leading zeros included); square array
 *          A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A253586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl, look
 * Formula:   square array A(n,k) is given by * A(0,0) = 0 * A(n,k) =
 *            3*A(floor(n/3),floor(k/3)) + ((6 - (n%3) - (k%3)) % 3) * for n,k
 *            ≥ 0. Sequence a(n) is the array read by antidiagonals: * a(n) =
 *            A(i,j) where i = n - d*(d+1)/2 and j = d - i * with d the unique
 *            integer such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2
 * Range:     u8 n <= 9636, u16 n <= 9636, u32 n <= 9636, u64 n <= 9636,
 *            u128 n <= 9636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t idx)
{
    /* ------------------------------------------------------------------ *
     *  1. Locate the diagonal `d` that contains the index `idx`.          *
     * ------------------------------------------------------------------ */
    idx_t d = 0;
    for (;;) {
        idx_t next = d + 1;
        unsigned __int128 val = (unsigned __int128)next * (next + 1) / 2;
        if (val <= idx)
            d = next;
        else
            break;
    }

    /* ------------------------------------------------------------------ *
     *  2. Translate the linear index `idx` into 2‑D coordinates (i,j).    *
     * ------------------------------------------------------------------ */
    unsigned __int128 start = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = idx - (idx_t)start;
    idx_t i = offset;
    idx_t j = d - offset;

    /* ------------------------------------------------------------------ *
     *  3. Compute A(i,j) from the ternary digits of i and j.              *
     * ------------------------------------------------------------------ */
    /* maximum number of ternary digits for a 64‑bit unsigned integer   */
    const unsigned int DMAX = 64;          /* safety margin                  */
    unsigned int rdigits[DMAX];
    unsigned int len = 0;

    idx_t ni = i;
    idx_t kj = j;
    while (ni > 0 || kj > 0) {
        unsigned int rn = ni % 3;
        unsigned int rk = kj % 3;
        unsigned int r  = (6 - rn - rk) % 3;
        rdigits[len++] = r;
        ni /= 3;
        kj /= 3;
    }

    /* assemble the base‑3 number formed by the collected digits */
    seq_t res = 0;
    if (len == 0)       /* only happens for i = j = 0 */
        return 0;

    for (int k = (int)len - 1; k >= 0; --k)
        res = res * 3 + rdigits[k];

    return res;
}
