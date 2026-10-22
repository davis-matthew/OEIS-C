/*
 * A334173  Least i such that i-th term of Thue-Morse sequence (A010060)
 *          differs from (i + n)-th term.
 * https://oeis.org/A334173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 2*a(n); a(2*n+1) = 0 if popcount(2*n+1) is odd, else
 *            a(2*n+1) = 1 if the run of trailing 1’s in 2*n+1 is odd, = 2 if
 *            that run is even. Equivalently: a(n) = 0 when popcount(n) is
 *            odd; otherwise a(n) = 2^k * (1 if trailing‑ones run of (n>>k)
 *            odd else 2), where k = #trailing zeros of n
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long x = (unsigned long long)n;

    /* 1. If the Thue–Morse value t(n)=1 (odd pop‑count), answer is 0. */
    if (__builtin_popcountll(x) & 1U)
        return (seq_t)0;

    /* 2. Count trailing zeros of n (the exponent k in a(2*n)=2*a(n)). */
    unsigned int k = __builtin_ctzll(x);

    /* 3. Remove those zeros to work with an odd argument. */
    unsigned long long odd = x >> k;

    /* 4. Count the run of consecutive 1‑bits at the LSB of odd. */
    unsigned int r = __builtin_ctzll(~odd);

    /* 5. For odd arguments: 1 when r odd, 2 when r even. */
    unsigned int val = (r & 1U) ? 1U : 2U;

    /* 6. Scale by 2^k and return. */
    return ((seq_t)val) << k;
}
