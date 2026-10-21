/*
 * A270827  a(n) is the smallest k such that k^8 = 16 (mod 2*n-1).
 * https://oeis.org/A270827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation: finds the smallest k ≥ 1 such that k^8 ≡ 16 (mod 2*n-1) by brute–force. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */
    /* modulus m = 2*n-1 (an odd integer) */
    seq_t mod = (seq_t)((idx_t)2 * n - 1);
    if (mod == 1) return (seq_t)1;        /* for m=1, any k works, minimal is 1 */

    seq_t target = (seq_t)16 % mod;       /* 16 modulo m */

    for (seq_t k = 1; k < mod; ++k) {
        seq_t t2 = (k * k) % mod;         /* k^2 mod m */
        seq_t t4 = (t2 * t2) % mod;       /* k^4 mod m */
        seq_t t8 = (t4 * t4) % mod;       /* k^8 mod m */
        if (t8 == target) return k;       /* minimal k found */
    }

    /* Theoretically unreachable: 16 is always an eighth power modulo any odd modulus. */
    return (seq_t)0;
}
