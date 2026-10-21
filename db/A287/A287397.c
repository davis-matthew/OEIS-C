/*
 * A287397  Start with 0 and repeatedly substitute 0->012, 1->210, 2->102.
 * https://oeis.org/A287397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>1 let k=n-1 (0‑based index).
   Let p = k/3 and r = k%3; then a(n) = M[a(p+1)][r] where
   M = {{0,1,2},{2,1,0},{1,0,2}}. */
static seq_t digit_at(idx_t idx) {
    if (idx == 0) return 0;
    idx_t parent = idx / 3;
    idx_t pos = idx % 3;
    seq_t pval = digit_at(parent);
    if (pval == 0) {
        return pos;                /* 0 -> {0,1,2} */
    } else if (pval == 1) {
        if (pos == 0) return 2;    /* 1 -> {2,1,0} */
        if (pos == 1) return 1;
        return 0;
    } else { /* pval == 2 */
        if (pos == 0) return 1;    /* 2 -> {1,0,2} */
        if (pos == 1) return 0;
        return 2;
    }
}

seq_t a(idx_t n) {
    return digit_at(n - 1);
}
