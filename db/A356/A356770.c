/*
 * A356770  a(n) is the number of equations in the set {x+2y=n, 2x+3y=n, ...,
 *          k*x+(k+1)*y=n, ..., n*x+(n+1)*y=n} which admit at least one
 *          nonnegative integer solution.
 * https://oeis.org/A356770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of k∈{1,…,n} for which kx + (k+1)y = n has a
 *            non‑negative integer solution
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the observation that the smallest non‑negative solution x₀ mod (k+1) is
   x₀ = (n·k) mod (k+1).  A solution exists iff x₀ ≤ ⌊n/k⌋. */
seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t mod = k + 1;                       /* k and k+1 are coprime */
        idx_t nm  = n % mod;                     /* n modulo (k+1)          */
        /* compute x0 = (nm * k) % mod using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)nm * (unsigned __int128)k;
        idx_t x0 = (idx_t)(prod % mod);
        if (x0 <= n / k)                         /* exists non‑negative solution */
            ++count;
    }
    return count;
}
