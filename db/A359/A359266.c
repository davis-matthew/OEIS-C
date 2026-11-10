/*
 * A359266  Numbers k such that A359194(k) > k.
 * https://oeis.org/A359266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=0; run j>=2 of consecutive terms has length A000975(j-1)
 *            and initial value A005578(j+1); a(n) found by walking the runs
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A359266_start(idx_t m)
{
    /* A005578(m): a(0)=1, a(2n)=2a(2n-1), a(2n+1)=2a(2n)-1 */
    seq_t v = 1;
    idx_t i;
    for (i = 1; i <= m; i++) {
        if (i % 2 == 0)
            v = v * 2;
        else
            v = v * 2 - 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t rem = (seq_t)n - 1;
    idx_t m = 1;          /* current run j = m+1 has length A000975(m) */
    seq_t L = 1;          /* A000975(1) */
    while (rem > L) {
        rem -= L;
        m++;
        L = L * 2 + (seq_t)(m & 1);
    }
    return A359266_start(m + 2) + rem - 1;
}
