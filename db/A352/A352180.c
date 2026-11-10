/*
 * A352180  a(n) = 9*A006190(n)+1.
 * https://oeis.org/A352180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*A006190(n) + 1, where A006190(n) = 3*A006190(n-1) +
 *            A006190(n-2), A006190(0) = 0, A006190(1) = 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A006190_a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)1;
    seq_t curr = (seq_t)1;

    for (idx_t i = 2; i <= n; i++) {
        curr = (seq_t)3 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}

seq_t a(idx_t n) {
    return (seq_t)9 * A006190_a(n) + (seq_t)1;
}
