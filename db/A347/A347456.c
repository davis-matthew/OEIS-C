/*
 * A347456  Number of factorizations of n with alternating product >= 1.
 * https://oeis.org/A347456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of weakly increasing factorizations of n with
 *            alternating product (product of odd‑position terms / product of
 *            even‑position terms) ≥ 1
 * Range:     u8 n <= 8639, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void dfs(idx_t rem, idx_t last, int parity, seq_t odd, seq_t even, seq_t *cnt)
{
    if (rem == 1) {
        if (odd >= even)
            ++(*cnt);
        return;
    }
    for (idx_t f = last; f <= rem; ++f) {
        if (rem % f != 0)
            continue;
        if ((parity & 1) == 0) {
            dfs(rem / f, f, parity + 1, odd * (seq_t)f, even, cnt);
        } else {
            dfs(rem / f, f, parity + 1, odd, even * (seq_t)f, cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1; /* empty factorization for n = 1 counts as 1 */

    seq_t count = 0;
    dfs(n, 2, 0, 1, 1, &count);
    return count;
}
