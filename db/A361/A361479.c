/*
 * A361479  a(n) is the least integer whose binary expansion has the same
 *          multiset of run-lengths as that of n.
 * https://oeis.org/A361479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least integer whose binary expansion has the same
 *            multiset of run‐lengths as that of n
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* find bit length of n */
    int bit_len = 0;
    idx_t tmp = n;
    while (tmp) {
        bit_len++;
        tmp >>= 1;
    }

    /* collect run lengths (from MSB to LSB) */
    idx_t runs[128];
    int run_cnt = 0;
    idx_t cur = (n >> (bit_len - 1)) & 1;
    int count = 1;
    for (int i = bit_len - 2; i >= 0; i--) {
        idx_t bit = (n >> i) & 1;
        if (bit == cur) {
            count++;
        } else {
            runs[run_cnt++] = count;
            cur = bit;
            count = 1;
        }
    }
    runs[run_cnt++] = count;  /* last run */

    /* sort run lengths ascending */
    for (int i = 1; i < run_cnt; i++) {
        int j = i;
        idx_t v = runs[i];
        while (j > 0 && runs[j - 1] > v) {
            runs[j] = runs[j - 1];
            j--;
        }
        runs[j] = v;
    }

    /* assign run lengths: smallest to 1‑runs (even indices), largest to 0‑runs (odd indices) */
    idx_t seq_len[128];
    int l = 0, r = run_cnt - 1;
    for (int i = 0; i < run_cnt; i++) {
        if ((i & 1) == 0)          /* 1‑run */
            seq_len[i] = runs[l++];
        else                       /* 0‑run */
            seq_len[i] = runs[r--];
    }

    /* build the resulting integer bit by bit */
    seq_t result = 0;
    for (int i = 0; i < run_cnt; i++) {
        for (int j = 0; j < seq_len[i]; j++) {
            result <<= 1;
            if ((i & 1) == 0)     /* 1‑run */
                result |= 1;
        }
    }
    return result;
}
