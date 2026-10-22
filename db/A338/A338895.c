/*
 * A338895  Three-column table read by rows giving Pythagorean triples [a,b,c]
 *          that are the (constant) differences between consecutive triples in
 *          rows of A338275.
 * https://oeis.org/A338895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338895: differences between consecutive triples in rows of A338275.
   For odd m ≥ 3, let x = m−1 (even), and let n run over even values
   2,4,…,x.  Each difference triple is

          a = ((x*x - n*n)/2),
          b =  (x * n),
          c = ((x*x + n*n)/2)

   Triples are output in order of increasing m and then increasing n.
   The flattened list of a, b, c values forms the sequence a(n). */

seq_t a(idx_t n) {
    /* Map n to triple index t (0‑based) and position within the triple. */
    if (n == 0) return 0;          /* out of range, not used in tests */
    idx_t t   = (n - 1) / 3;       /* 0‑based triple index */
    idx_t pos = (n - 1) % 3;        /* 0 → a, 1 → b, 2 → c */

    /* Find the odd m that contains the t‑th triple. */
    idx_t cumulative = 0;          /* number of triples processed so far */
    idx_t m = 3;                   /* start with the smallest odd m ≥ 3 */
    for (;;) {
        idx_t x   = m - 1;         /* x is even */
        idx_t count = x / 2;       /* number of triples for this m  */
        if (cumulative + count > t) {
            break;                /* the desired triple is in this row */
        }
        cumulative += count;
        m += 2;                    /* next odd m */
    }

    /* Determine the n (even) for the t‑th triple within the row. */
    idx_t offset = t - cumulative;        /* 0‑based within this row */
    idx_t n_even = 2 * (offset + 1);      /* n = 2,4,…,x */

    /* Compute the triple in seq_t to support arbitrary precision. */
    seq_t x_seq = (seq_t)(m - 1);
    seq_t n_seq = (seq_t)n_even;

    seq_t a_val = ((x_seq * x_seq) - (n_seq * n_seq)) / 2;
    seq_t b_val = x_seq * n_seq;
    seq_t c_val = ((x_seq * x_seq) + (n_seq * n_seq)) / 2;

    /* Return the requested component. */
    if (pos == 0) return a_val;
    if (pos == 1) return b_val;
    return c_val;                    /* pos == 2 */
}
