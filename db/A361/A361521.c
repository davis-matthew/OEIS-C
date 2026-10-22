/*
 * A361521  Array read by descending antidiagonals. A(n, k) is the number of
 *          the nonempty multiset combinations of {0, 1} as defined in
 *          A361682.
 * https://oeis.org/A361521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(p,k) where d = floor((sqrt(8n+1)-1)/2), p = n -
 *            d(d+1)/2, k = d - p. A(p,k) = p*k*(4 + p*(k-1))/2. This reads
 *            the A(n,k) array along descending anti‑diagonals with ascending
 *            n inside each diagonal
 * Range:     u8 n <= 58, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t diagonal_of(idx_t n) {
    /* Find the largest d with d(d+1)/2 <= n.
       d never exceeds 2^32 for 64‑bit n. */
    const idx_t MAX_D = (1ULL << 32);
    idx_t lo = 0, hi = MAX_D, best = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            best = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n) {
    idx_t d = diagonal_of(n);
    idx_t start = d * (d + 1) / 2;
    idx_t p = n - start;          /* n coordinate inside diagonal */
    idx_t k = d - p;              /* k coordinate inside diagonal */

    if (p == 0 || k == 0) return 0;  /* A(p,k)=0 when p or k is 0 */

    seq_t fn = (seq_t)p;
    seq_t fk = (seq_t)k;

    /* Compute A(p,k) = p*k*(4 + p*(k-1))/2 */
    seq_t term = fn * fk * (seq_t)(4 + fn * (fk - 1)) / 2;
    return term;
}
