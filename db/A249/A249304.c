/*
 * A249304  Number of zeros in row n of triangle A249133.
 * https://oeis.org/A249304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048967(n) + A048967(n-1) with A048967(0)=A048967(1)=0
 *            and for n>1 if n is odd: A048967(n) = 2*A048967((n-1)/2) if n is
 *            even: A048967(n) = 2*A048967(n/2) + (2^popcount(n/2)-1)
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Determine popcount of an idx_t value */
static inline unsigned popcount_idx(idx_t v) {
    /* Treat idx_t as 64‑bit for builtin */
    return __builtin_popcountll((unsigned long long)v);
}

/* Recursive computation of A048967(n) */
static seq_t A048967(idx_t n) {
    if (n <= 1) return 0;                 /* A048967(0)=A048967(1)=0 */

    if (n & 1) {                          /* n is odd */
        return 2 * A048967((n - 1) >> 1);
    } else {                              /* n is even */
        idx_t h = n >> 1;                 /* h = n/2 */
        seq_t part = 2 * A048967(h);
        unsigned pc = popcount_idx(h);
        seq_t pow = ((seq_t)1 << pc) - 1;  /* 2^popcount(h) – 1 */
        return part + pow;
    }
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    return A048967(n) + A048967(n - 1);
}
