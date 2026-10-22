/*
 * A324572  Number of integer partitions of n whose multiplicities (where if x
 *          < y the multiplicity of x is counted prior to the multiplicity of
 *          y) are equal to the distinct parts in decreasing order.
 * https://oeis.org/A324572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer partitions of n whose multiplicities (counted
 *            in increasing order of parts) are equal to the distinct parts in
 *            decreasing order
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------- */
/* Helper: integer square root for idx_t (returns floor(sqrt(x)))       */
/* -------------------------------------------------------------------- */
static idx_t int_sqrt(idx_t n) {
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        uint64_t sq = (uint64_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) { ans = mid; low = mid + 1; }
        else        high = mid - 1;
    }
    return ans;
}

/* -------------------------------------------------------------------- */
/* Recursive enumeration of self‑dual partitions inside interval (L,R). */
/* L and R are exclusive bounds (all used part values satisfy
   L < value < R).  rem is the remaining sum to be formed.            */
/* -------------------------------------------------------------------- */
static seq_t dfs(idx_t L, idx_t R, idx_t rem) {
    if (rem == 0) return 1;      /* empty sub‑partition */

    seq_t total = 0;

    /* Middle element case (odd number of distinct parts) */
    idx_t m = int_sqrt(rem);
    if ((seq_t)m * m == rem && m > L && m < R) {
        total += 1;
    }

    /* Choose a pair (l,r) with L < l < r < R          */
    for (idx_t l = L + 1; l < R - 1; ++l) {
        /* maximal r allowed by the remaining sum */
        idx_t max_r = rem / (2 * l);
        if (max_r > R - 1) max_r = R - 1;
        if (max_r <= l) continue;

        idx_t w_base = 2 * l;
        for (idx_t r = l + 1; r <= max_r; ++r) {
            idx_t weight = w_base * r;        /* 2*l*r */
            total += dfs(l, r, rem - weight);
        }
    }
    return total;
}

/* -------------------------------------------------------------------- */
/* Public function required by the build system                          */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* maximal part that can appear in a pair */
    idx_t max_pair = n / 2;
    /* maximal part that can appear as an odd middle element */
    idx_t max_mid  = int_sqrt(n);
    if (max_mid > max_pair) max_pair = max_mid;
    /* exclusive upper bound for the outermost call */
    idx_t R = max_pair + 1;

    return dfs(0, R, n);
}
