/*
 * A286899  Array read by antidiagonals: A(n, L) is the number of closed walks
 *          of length 2L along the edges of an n-cube based at a vertex, for n
 *          >= 1 and L >= 1.
 * https://oeis.org/A286899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 58, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A286899 – array read by antidiagonals of A(n,L)   */
/*  a(n) = sum_{i=0}^{n'} C(n',i)*(n'‑2i)^(2L') / 2^n',   */
/*  where (n',L') is the position of the n‑th element in the
 *  antidiagonal ordering.                                   */

#include <stdint.h>

/* Local helpers – no external utilities are required */
static seq_t my_binomial(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (unsigned __int128)(n - k + i);
        res = res / (unsigned __int128)i;
    }
    return (seq_t)res;
}

static seq_t power_seq(seq_t base, seq_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = (seq_t)((unsigned __int128)result * base);
        exp >>= 1;
        if (exp) base = (seq_t)((unsigned __int128)base * base);
    }
    return result;
}

/* Find the antidiagonal number d such that
 *  (d-1)d/2 < N <= d(d+1)/2                               */
static idx_t diagonal_of(idx_t N) {
    idx_t low = 1, high = N;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= N)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* 1‑based index into the antidiagonal ordering */
    idx_t d = diagonal_of(n);                     /* diagonal number   */
    idx_t prev_cnt = (d - 1) * d / 2;              /* elements before this diagonal */
    idx_t offset = n - prev_cnt;                  /* 1‑based position within diagonal */

    idx_t n_prime = offset;                       /* first argument of A   */
    idx_t L_prime = d - offset + 1;              /* second argument of A  */

    seq_t sum = 0;
    for (idx_t i = 0; i <= n_prime; ++i) {
        seq_t bin = my_binomial(n_prime, i);

        /* absolute value of (n_prime - 2*i) */
        long long signed_shift = (long long)n_prime - (long long)(2 * i);
        long long abs_shift  = signed_shift >= 0 ? signed_shift : -signed_shift;
        seq_t base = (seq_t)abs_shift;

        seq_t exp = (seq_t)(2 * L_prime);
        seq_t pwr = power_seq(base, exp);

        sum += bin * pwr;
    }

    seq_t denom = ((seq_t)1) << n_prime;          /* 2^n'                */
    return sum / denom;
}
