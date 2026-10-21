/*
 * A265583  Array T(n,k) = k*(k-1)^(n-1) read by ascending antidiagonals; k,n
 *          >= 1.
 * https://oeis.org/A265583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 70, u32 n <= 176, u64 n <= 442,
 *            u128 n <= 1139, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) reads the array T(n,k)=k*(k-1)^(n-1) along ascending antidiagonals.
 * Let s be the diagonal index such that s*(s+1)/2 ≥ n > (s-1)*s/2.
 * The position p within this diagonal is n-(s-1)*s/2 (1‑based).
 * Then k = p and n_len = s-p+1, so the term is k*(k-1)^(n_len-1). */

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* find diagonal s satisfying s*(s+1)/2 >= n */
    idx_t s = 1;
    while (((unsigned __int128)s * (s + 1)) / 2 < n) s++;

    /* number of terms before this diagonal */
    idx_t prev = ((unsigned __int128)(s - 1) * s) / 2;

    /* position within diagonal (1‑based) */
    idx_t p = n - prev;              /* 1 ≤ p ≤ s */

    /* column index k and row index n_len */
    idx_t k = p;                     /* 1 ≤ k ≤ s */
    idx_t n_len = s - p + 1;         /* 1 ≤ n_len ≤ s */

    /* compute base^(exp) with base = k-1 and exp = n_len-1 */
    seq_t base = (seq_t)(k - 1);
    seq_t exp  = (seq_t)(n_len - 1);
    seq_t power;

    if (base == 0) {
        /* 0^0 must be treated as 1 */
        power = (exp == 0) ? (seq_t)1 : (seq_t)0;
    } else {
        power = (seq_t)1;
        for (idx_t i = 0; i < (idx_t)exp; ++i) power *= base;
    }

    return (seq_t)k * power;
}
