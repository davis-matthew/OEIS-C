/*
 * A368526  a(n) = Sum_{k=1..n} k^2 * n^k.
 * https://oeis.org/A368526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^n*(n^3-3*n^2+2*n+1)-1)/(n-1)^3, a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local power routine for arbitrary‑precision seq_t (no overflow handling) */
static seq_t seq_pow_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* For 64‑bit seq_t we use unsigned __int128 to avoid overflow
       while keeping the final result in 64 bits. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 N = n;
        unsigned __int128 n_pow_n = 1;
        for (idx_t i = 0; i < n; ++i) n_pow_n *= N;

        unsigned __int128 poly   = N*N*N - 3*N*N + 2*N + 1;
        unsigned __int128 prod   = n_pow_n * poly - 1;
        unsigned __int128 num    = N * (N + 1) * prod;
        unsigned __int128 den    = (N - 1)*(N - 1)*(N - 1);
        unsigned __int128 res    = num / den;
        return (seq_t)res;           /* cast back to seq_t (uint64_t) */
    }

    /* For arbitrary‑precision seq_t we rely on the type’s intrinsic
       arithmetic; all intermediate values fit because the result is
       required only when it actually fits in the target type. */
    seq_t N   = (seq_t)n;
    seq_t pow_n = seq_pow_local(N, n);
    seq_t poly  = N*N*N - 3*N*N + 2*N + 1;
    seq_t prod  = pow_n * poly - 1;
    seq_t num   = N * (N + 1) * prod;
    seq_t den   = (N - 1)*(N - 1)*(N - 1);
    return num / den;
}
