/*
 * A295366  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) - b(n-3), where a(0) = 1, a(1) = 2, a[2] = 3, b(0)
 *          = 4, b(1) = 5, b(2) = 6, and (a(n)) and (b(n)) are increasing
 *          complementary sequences.
 * https://oeis.org/A295366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1) + b(n-2) - b(n-3) with a(0)=1,
 *            a(1)=2, a(2)=3, b(0)=4, b(1)=5, b(2)=6, and (a) and (b) are
 *            complementary increasing sequences
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* static buffers sufficient for n up to 10000 */
    static seq_t a_vals[10001];
    static seq_t b_vals[10001];

    /* current lengths of the sequences */
    static idx_t a_len = 3, b_len = 3;

    /* last value of b used to find the next unused integer */
    static seq_t last_b = (seq_t)6;

    /* pointers for fast membership tests while scanning forward */
    static idx_t ptr_a = 0, ptr_b = 0;

    /* one‑time initialization */
    static int initialized = 0;
    if (!initialized) {
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)2;
        a_vals[2] = (seq_t)3;
        b_vals[0] = (seq_t)4;
        b_vals[1] = (seq_t)5;
        b_vals[2] = (seq_t)6;
        initialized = 1;
    }

    /* Return early if the requested term is already known */
    if (n < a_len) return a_vals[n];

    /* Generate terms up to index n */
    for (idx_t i = a_len; i <= n; ++i) {
        /* ---- (1) compute the next unused integer as b(i) ---- */
        seq_t cand = last_b + (seq_t)1;
        for (;;) {
            /* Advance ptr_a and ptr_b to the first entries >= cand */
            while (ptr_a < a_len && a_vals[ptr_a] < cand) ++ptr_a;
            while (ptr_b < b_len && b_vals[ptr_b] < cand) ++ptr_b;

            /* Skip if the candidate is already used */
            if (ptr_a < a_len && a_vals[ptr_a] == cand) {
                ++cand; ++ptr_a; continue;
            }
            if (ptr_b < b_len && b_vals[ptr_b] == cand) {
                ++cand; ++ptr_b; continue;
            }
            /* cand is free */
            break;
        }
        b_vals[b_len++] = cand;
        last_b = cand;

        /* ---- (2) compute a(i) for i>=3 ---- */
        if (i >= 3) {
            seq_t sum = a_vals[i-1];
            sum += a_vals[i-2];
            sum += b_vals[i-1];
            sum += b_vals[i-2];
            sum -= b_vals[i-3];
            a_vals[i] = sum;
        }
        ++a_len;
    }

    return a_vals[n];
}
