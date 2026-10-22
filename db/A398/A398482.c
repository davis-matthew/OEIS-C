/*
 * A398482  Triangle read by rows: T(n, k) = n^k * (1 + n)^(n - k).
 * https://oeis.org/A398482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r^k * (r+1)^(r-k), where r is the unique integer with
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n - r(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exponentiation by squaring – works for all unsigned seq_t types,
   whether integer or arbitrary‑precision. */
static inline seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Compute the nth term of OEIS A398482. */
seq_t
a(idx_t n)
{
    /* Find r: the largest integer with r(r+1)/2 ≤ n. */
    idx_t low = (idx_t)0;
    idx_t high = n;                     /* inclusive upper bound */
    while (low < high) {
        /* Use upper mid to avoid infinite loop when low+1==high. */
        idx_t mid = low + (high - low + (idx_t)1) / (idx_t)2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + (idx_t)1) / (idx_t)2;
        if (tri <= n)
            low = mid;
        else
            high = mid - (idx_t)1;
    }
    idx_t r = low;
    unsigned __int128 tri_r = (unsigned __int128)r * (r + (idx_t)1) / (idx_t)2;
    idx_t k = n - (idx_t)tri_r;          /* 0 ≤ k ≤ r */

    /* Compute r^k * (r+1)^(r-k). */
    seq_t part1 = pow_seq((seq_t)r, k);
    seq_t part2 = pow_seq((seq_t)(r + (idx_t)1), r - k);
    return part1 * part2;
}
