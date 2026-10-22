/*
 * A307684  Total volume of all rectangular prisms with dimensions r X s X t
 *          where r, s and t are the smallest, middle and largest parts in
 *          each partition of n into 3 parts.
 * https://oeis.org/A307684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formel: a(n) = Σ_{k=1..⌊n/3⌋} Σ_{i=k..⌊(n-k)/2⌋} i * k * (n-i-k) */
/* Total volume of all rectangular prisms with dimensions obtained
   from partitioning n into 3 parts (r≤s≤t) */

seq_t a(idx_t n)
{
    if (n < 3) return 0;           /* for n = 1, 2 the sum is empty */

    seq_t total = 0;
    idx_t max_k = n / 3;           /* floor(n/3) */

    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t rk = (seq_t)k;       /* r = k */
        idx_t max_i = (n - k) / 2; /* floor((n-k)/2) */
        for (idx_t i = k; i <= max_i; ++i) {
            seq_t sk = (seq_t)i;   /* s = i */
            seq_t tk = (seq_t)(n - i - k); /* t = n-i-k */
            total += sk * rk * tk;
        }
    }
    return total;
}
