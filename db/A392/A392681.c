/*
 * A392681  Square array read by upward antidiagonals: T(n,k) = sum of depths
 *          of leaves in a complete k-ary tree with n nodes; n >= 1, k >= 2.
 * https://oeis.org/A392681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(n_val, k_val) where T(n,k) = (n - M) + (h-1)*L with M =
 *            (k^h - 1)/(k-1), h = floor(log_k(n*(k-1))), L = n - floor((n + k
 *            - 2)/k)
 * Range:     u8 n <= 2503, u16 n <= 8765, u32 n <= 8765, u64 n <= 8765,
 *            u128 n <= 8765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t total = 0;
    idx_t d = 3;               /* diagonal index (n + k) starts at 3 */
    for (;;) {
        total += d - 2;        /* each diagonal contains d-2 elements */
        if (total >= n)
            break;
        d++;
    }
    idx_t prev_total = total - (d - 2);
    idx_t offset = n - prev_total - 1;  /* 0‑based position in diagonal d */
    idx_t n_val = (d - 2) - offset;     /* n in the tree */
    idx_t k_val = d - n_val;            /* k in the tree */

    if (n_val == 1)                   /* tree with a single node */
        return 0;

    seq_t n_seq = (seq_t)n_val;
    seq_t k_seq = (seq_t)k_val;

    /* h = floor(log_k(n*(k-1))) */
    seq_t product = n_seq * (k_seq - 1);
    seq_t pow_k_h = 1;
    idx_t h = 0;
    while (pow_k_h * k_seq <= product) {
        pow_k_h *= k_seq;
        h++;
    }

    /* M = (k^h - 1)/(k-1) */
    seq_t M = (pow_k_h - 1) / (k_seq - 1);

    /* L = n - floor((n + k - 2)/k) */
    seq_t L = n_seq - ((n_seq + k_seq - 2) / k_seq);

    seq_t res = n_seq - M;
    if (h > 0)
        res += ((seq_t)(h - 1)) * L;
    return res;
}
