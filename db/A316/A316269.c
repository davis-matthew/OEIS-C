/*
 * A316269  Array T(n,k) = n*T(n,k-1) - T(n,k-2) read by upward antidiagonals,
 *          with T(n,0) = 0, T(n,1) = 1, n >= 2.
 * https://oeis.org/A316269
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 86, u32 n <= 184, u64 n <= 455,
 *            u128 n <= 1163, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k)=n*T(n,k-1)-T(n,k-2),  T(n,0)=0, T(n,1)=1.
   Sequence a(n) (offset 2) is read from the table by upward antidiagonals:
   a(2)=T(2,0), a(3)=T(3,0), a(4)=T(2,1), a(5)=T(4,0), a(6)=T(3,1), ...   */
seq_t a(idx_t idx)
{
    /* Off‑by‑one: a(2) is the first term, so use a 0‑based index i */
    if (idx < 2) return 0;              /* undefined for indices <2, return 0 */
    idx_t i = idx - 2;                  /* i = 0 for a(2) */

    /* Find the antidiagonal (sum s = n + k) containing the index i.
       The number of terms up to diagonal s-1 is (s-2)*(s-1)/2. */
    idx_t s = 2;
    while (((unsigned __int128)(s - 1) * s) / 2 <= i) {
        ++s;
    }

    /* Number of terms before diagonal s */
    idx_t prev_cum = ((unsigned __int128)(s - 2) * (s - 1)) / 2;
    idx_t j = i - prev_cum;              /* position within diagonal, 0‑based */
    seq_t n_val = (seq_t)(s - j);        /* n = s – j   */
    idx_t k_val = j;                     /* k = j       */

    /* Compute T(n_val, k_val) iteratively (k <= s-2 is small). */
    if (k_val == 0) return 0;
    if (k_val == 1) return 1;

    seq_t prev2 = 0;                     /* T(n,0) */
    seq_t prev1 = 1;                     /* T(n,1) */
    seq_t mult = n_val;                  /* n as seq_t */
    for (idx_t t = 2; t <= k_val; ++t) {
        seq_t cur = mult * prev1 - prev2;/* T(n,k) = n*T(n,k-1) - T(n,k-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
